/*
#include "include/server.hpp"
#include "include/Command/Connexion.hpp"
#include "include/Command/SendToClient.hpp"
#include "include/Command/Entity.hpp"

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

Entity &Server::MakeEntity(uint16_t configurationId) {
    std::string path = find_entity_path(configurationId);
    Entity &e = em.createEntity();
    maker->parseJson(e, path);
    e.setConfigId(configurationId);
    return e;
}

void Server::destroy_entity(Entity e){
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

void Server::run() {
    using Clock = std::chrono::steady_clock;
    auto lastTime = Clock::now();
    double time = 0;
    sf::Clock clock;
    double time2 = 0;

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
            std::cerr << "[SERVER] Handle MEssage Error: " << e.what() << '\n';
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
            std::cerr << "[SERVER] Script Update Mouvement Error: " << e.what() << '\n';
        }

        try {
            script.updateAttack(maker->scripting, maker->attack, maker->pos, deltaTime,
                std::function<void(float, float)>([this](float x, float y) {
                    Entity e = this->MakeEntity(2);
                    maker->UpdatePosition(e, x, y);
                    maker->UpdateGroup(e, "ennemy");
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

        if (time2 > 10) {
            this->MakeEntity(4);
            time2 = 0;
        }
    }
}
*/
