#include "include/server.hpp"
#include "include/Command/Connexion.hpp"
#include "include/Command/SendToClient.hpp"
#include "include/Command/Entity.hpp"

/**
 * @file server.cpp
 * @brief Implementation of the Server class for server management.
 */

using entityToMake = std::tuple<uint16_t, float, float, float>;

std::vector<entityToMake> load_level(const std::string &path){
    std::ifstream file(path);
    if (!file.is_open())
        throw JsonParseException("Could not open file: " + path);
    if (file.peek() == std::ifstream::traits_type::eof())
        throw JsonParseException("File is empty: " + path);
    nlohmann::json json;
    file >> json;

    std::vector<entityToMake> entities;
    for (const auto& entity : json["entities"]) {
        float pos_x = entity["position"]["x"];
        float pos_y = entity["position"]["y"];
        float spawnTime = entity["spawntime"];
        uint16_t configID = entity["configID"];
        entities.push_back(std::make_tuple(configID, pos_x, pos_y, spawnTime));
    }
    return entities;
}

/**
 * @brief Constructor of the Server class.
 * Initializes the server with a given port and configures the entity components.
 *
 * @param port The port listened to by the server.
 */
Server::Server(int32_t port) : server(port) {
    this->maker = std::make_unique<EntityMaker>();

    maker->acc = reg.register_component<Acceleration>();
    maker->pos = reg.register_component<Position>();
    maker->rot = reg.register_component<Rotation>();
    maker->scale = reg.register_component<Scale>();
    maker->velo = reg.register_component<Velocity>();
    maker->attack = reg.register_component<Attack>();
    maker->group = reg.register_component<Group>();
    maker->health = reg.register_component<Health>();
    maker->hitbox = reg.register_component<Hitbox>();
    maker->lifetime = reg.register_component<Component::LifeTime>();
    maker->scripting = reg.register_component<Scripting>();
}

/**
 * @brief Creates an entity based on a configuration ID.
 *
 * @param configurationId The configuration ID of the entity.
 * @return A reference to the created entity.
 */
Entity &Server::MakeEntity(uint16_t configurationId) {
    std::string path = find_entity_path(configurationId);
    Entity &e = em.createEntity();
    maker->parseJson(e, path);
    e.setConfigId(configurationId);
    return e;
}

/**
 * @brief Destroys an entity and cleans up its components.
 *
 * @param e The entity to destroy.
 */
void Server::destroy_entity(Entity e) {
    Message<Communication::TypeDetail> msg;

    msg.header.type = {Communication::EntityInfo, Communication::main::EntityInfoPrecision::DeleteEntity};
    msg.header.size = 0;

    uint16_t id = e.getId();
    msg << id;
    this->em.destroyEntity(e);

    maker->acc.erase(e);
    maker->pos.erase(e);
    maker->rot.erase(e);
    maker->scale.erase(e);
    maker->velo.erase(e);
    maker->attack.erase(e);
    maker->group.erase(e);
    maker->health.erase(e);
    maker->hitbox.erase(e);
    maker->lifetime.erase(e);
    maker->scripting.erase(e);

    this->server.SendAll(msg);
}

/**
 * @brief Handles a message received from a given endpoint.
 *
 * @param msg The received message.
 * @param endpoint The endpoint of the user sending the message.
 */
void Server::handleMessage(Message<Communication::TypeDetail> &msg, const asio::ip::udp::endpoint &endpoint) {
    User user;

    try {
        user = server.GetUser(endpoint);
    } catch(const std::runtime_error &e) {
        user.endpoint = endpoint;
        server.AddUser(user);
    }

    switch (msg.header.type.type) {
        case Communication::ConnexionDetail:
            handleGlobalConnexion(msg, user, *this);
            break;

        case Communication::EntityAction:
            handleGlobalEntity(msg, user, *this);
            break;
        default:
            break;
    }
}

/**
 * @brief Starts the main loop of the server.
 *
 * Manages entities, messages, collisions, and scripts in a continuous loop while the server is active.
 */
void Server::run() {
    using Clock = std::chrono::steady_clock;
    auto lastTime = Clock::now();
    double time = 0;
    sf::Clock clock;
    double time2 = 0;

    std::vector<entityToMake> entities = load_level("assets/levels/level2.json");
    while (this->isRunning) {
        auto currentTime = Clock::now();
        std::chrono::duration<double> elapsed = currentTime - lastTime;
        float deltaTime = clock.restart().asSeconds();
        lastTime = currentTime;
        time += deltaTime;
        time2 += deltaTime;

        try {
            for (std::optional<std::pair<asio::ip::udp::endpoint, Message<Communication::TypeDetail>>> msg = server.Receive(); msg; msg = server.Receive()) {
                handleMessage(msg->second, msg->first);
            }
        } catch(const std::exception& e) {
            std::cerr << "[SERVER] Handle Message Error: " << e.what() << '\n';
        }

        try {
            this->movement.update(maker->pos, maker->velo, deltaTime);
        } catch(const std::exception& e) {
            std::cerr << "[SERVER] Movement Error: " << e.what() << '\n';
        }

        try {
            for (size_t i = 0; i < maker->attack.size(); i++) {
                if (maker->attack[i].has_value())
                    this->maker->attack[i].value().update(deltaTime);
            }
        } catch (const std::exception &e) {
            std::cerr << "[SERVER] Attack Error: " << e.what() << std::endl;
        }

        try {
            collision.update(maker->pos, maker->hitbox,
                std::function<bool(Entity, Entity)>([this](Entity e1, Entity e2) {
                    if (maker->group[e1].has_value() && maker->group[e2].has_value()){
                        if (!maker->group[e1].value().has(maker->group[e2].value().getGroups())) {
                                this->destroy_entity(e1);
                                this->destroy_entity(e2);
                                return true;
                            }
                        }
                        return false;
                    }
                )
            );
        } catch (const std::exception &e) {
            std::cerr << "[SERVER] Collision Error: " << e.what() << std::endl;
        }

        try {
            script.updateMovement(maker->scripting, maker->velo, maker->pos);
        } catch(const std::exception& e) {
            std::cerr << "[SERVER] Script Update Movement Error: " << e.what() << '\n';
        }

        try {
            script.updateAttack(maker->scripting, maker->attack, maker->pos, deltaTime,
                std::function<void(float, float)>([this](float x, float y) {
                    Entity e = this->MakeEntity(2);
                    maker->UpdatePosition(e, x, y);
                    maker->UpdateGroup(e, "enemy");
                    maker->InverseEntityX(e);
                })
            );
        } catch(const std::exception& e) {
            std::cerr << "[SERVER] Script Update Attack Error: " << e.what() << '\n';
        }

        try {
            std::vector<Entity> vec = this->lifetimeSystem.update(maker->lifetime, deltaTime);
            for (auto &e : vec) {
                destroy_entity(e);
            }
        } catch (const std::exception &e) {
            std::cerr << "[SERVER] Lifetime Error: " << e.what() << std::endl;
        }

        try {
            if (time > 0.002) {
                for (auto entity : em.getActiveEntities()) {
                    sendEntityInfo(entity, *this);
                }
                time = 0;
            }
        } catch(const std::exception& e) {
            std::cerr << "[SERVER] Sending Entity Error: " << e.what() << '\n';
            time = 0;
        }

        for (auto &entity : entities) {
            std::get<3>(entity) -= deltaTime;
            if (std::get<3>(entity) <= 0) {
                try {
                    Entity e = this->MakeEntity(std::get<0>(entity));
                    this->maker->UpdatePosition(e, std::get<1>(entity), std::get<2>(entity));
                    this->maker->InverseEntityX(e);
                    entities.erase(std::remove(entities.begin(), entities.end(), entity), entities.end());
                } catch (const std::exception& e) {
                    std::cerr << "Error Ennemy cannot be created" << std::endl;
                }
            }
        }
    }
}
