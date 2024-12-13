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

    while (this->isRunning) {
        auto currentTime = Clock::now();
        std::chrono::duration<double> elapsed = currentTime - lastTime;
        double deltaTime = elapsed.count();
        lastTime = currentTime;
        time += deltaTime;

        for (std::optional<std::pair<asio::ip::udp::endpoint, Message<Communication::TypeDetail>>> msg = server.Receive(); msg; msg = server.Receive()) {
            handleMessage(msg->second, msg->first);
        }

        this->movement.update(maker->pos, maker->velo, deltaTime);
        try {
            
            for (size_t i = 0; i < maker->attack.size(); i++) {
                if (maker->attack[i].has_value())
                    this->maker->attack[i].value().update(deltaTime);
            }
        } catch (const std::exception &e) {
            std::cerr << "gegrege: " << e.what() << std::endl;
        }
        this->lifetimeSystem.update(maker->lifetime, deltaTime);
        for (auto entity : em.getActiveEntities()) {
            sendEntityInfo(entity, *this);
        }
    }
}
