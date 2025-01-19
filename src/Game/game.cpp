#include "./game.hpp"
#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "src/Components/Velocity.hpp"
#include "src/Components/Hitbox.hpp"
#include "src/Components/Life.hpp"
#include "src/Components/Chrono.hpp"
#include "src/Components/Metadata.hpp"
#include "src/Components/Buff.hpp"
#include "src/Config/EntityConfigResolver.hpp"
#include "src/Config/ConfigurationIdResolver.hpp"
#include "src/Components/Action.hpp"

#define RTYPE_ECS_ENTITY_LIMIT 2500
namespace RType {
    Games::Games(std::string &ip, uint16_t TCPPort, uint16_t UDPPort, std::string gameName) : _GameServerTCP(TCPPort, gameName), _GameServerUDP(UDPPort, gameName), _levelManager(nullptr) {
        Rengine::UUID::generateUUID(_gameID);
        _ecs = std::make_shared<Rengine::ECS>(RTYPE_ECS_ENTITY_LIMIT);
        _levelManager.setECS(_ecs);
        std::function<void(Rengine::Entity&)> onDelete = [this](Rengine::Entity &entity) {
            Message<Communication::TypeDetail> msg;
            msg.header.type = {Communication::Type::EntityInfo, Communication::main::EntityInfoPrecision::DeleteEntity};
            msg.header.size = 0;
            Rengine::Entity::size_type id = entity;
            msg << id;
            this->_GameServerTCP.SendAll(msg);
        };
        _ecs->setOnEntityRemovalFunction<>(onDelete);
        _TCPPort = _GameServerTCP.getPort();
        _UDPPort = _GameServerUDP.getPort();
        _gameName = gameName;
        _ip = ip;
        _timeStarted = time(NULL);
        _running = false;
    }

    void Games::start() {
        this->_ecs->registerComponent<Components::Action>();
        this->_ecs->registerComponent<Components::Configuration>();
        this->_ecs->registerComponent<Components::Position>();
        this->_ecs->registerComponent<Components::Buff>();
        this->_ecs->registerComponent<Components::Hitbox>();
        this->_ecs->registerComponent<Components::Relationship>();
        this->_ecs->registerComponent<Components::Metadata>();
        this->_ecs->registerComponent<Components::Velocity>();
        this->_ecs->registerComponent<Components::Chrono>();
        this->_ecs->registerComponent<Components::Life>();
        this->initLevel();

        // Function
        std::function<void(Rengine::ECS&, Components::Action&, Rengine::Entity&, std::shared_ptr<Rengine::ECS>&)> actionFunction = Components::Action::componentFunction;
        this->_ecs->setComponentFunction<Components::Action, std::shared_ptr<Rengine::ECS>&>(actionFunction);
        this->_ecs->setComponentFunction<Components::Velocity>(Components::Velocity::componentFunction);
        this->_ecs->setComponentFunction<Components::Hitbox>(Components::Hitbox::componentFunction);
        this->_ecs->setComponentFunction<Components::Chrono>(Components::Chrono::componentFunction);
        this->_ecs->setComponentFunction<Components::Life>(Components::Life::componentFunction);
        _timeStarted = time(NULL);
        _running = true;
        _gameThread = std::thread(&Games::run, this);
    }

    bool Games::isRunning() {
        return _running;
    }

    void Games::initLevel(void)
    {
        // getting random level config
        std::vector<std::string> configVector;
        uint64_t idx = 0;

        // Loading a random level
        for (auto file : std::filesystem::directory_iterator("assets/levels/")) {
            if (file.path().string() == "assets/levels/id.json") {
                continue;
            }
            configVector.push_back(file.path().string());
        }
        // On error
        if (configVector.size() == 0) {
            this->_levelManager.loadLevel("assets/levels/template.json");
            return;
        }
        idx = Rengine::RNG::rngFunction() % configVector.size();
        this->_levelManager.loadLevel(configVector[idx]);
    }

    void Games::stop() {
        _running = false;
        _gameThread.join();
    }

    time_t &Games::getTimeStarted() {
        return _timeStarted;
    }

    Rengine::UUID::uuid_t &Games::getGameID() {
        return _gameID;
    }

    std::string &Games::getIP() {
        return _ip;
    }

    uint16_t &Games::getTCPPort() {
        return _TCPPort;
    }

    uint16_t &Games::getUDPPort() {
        return _UDPPort;
    }

    size_t Games::getNbUsers() {
        return _users.size();
    }

    const char *Games::getGameName() {
        return _gameName.c_str();
    }

    const std::string& Games::getLevelName(void) const noexcept {
        return this->_levelManager.getLevelName();
    }

    std::shared_ptr<userGame> Games::_getUserByClient(std::shared_ptr<Connexion<Communication::TypeDetail>> client) {
        for (std::shared_ptr<userGame> user : _users) {
            if (user->client == client)
                return user;
        }
        return nullptr;
    }

    std::shared_ptr<userGame> Games::_getUserByUDPClient(asio::ip::udp::endpoint &UDPClient) {
        for (std::shared_ptr<userGame> user : _users) {
            if (user->user.endpoint == UDPClient)
                return user;
        }
        return nullptr;
    }

    std::shared_ptr<userGame> Games::_getUserByTCPEndpoint(asio::ip::tcp::endpoint &TCPEndpoint) {
        for (std::shared_ptr<userGame> user : _users) {
            if (user->client->getSocket().remote_endpoint() == TCPEndpoint)
                return user;
        }
        return nullptr;
    }

    void Games::run() {
        while (_running) {
            try {
                while (Rengine::Clock::getElapsedTime() < 0.016f) {}
                this->_levelManager.updateDeltatime();
                if (this->_levelManager.isCurrentSceneOver()) {
                    if (!this->_levelManager.nextScene()) {
                        this->_levelManager.completeClear();
                        std::cout << "Level is over" << std::endl;
                        Message<Communication::TypeDetail> broadCastLevelEnd;
                        broadCastLevelEnd.header.type = {Communication::Type::ConnexionDetail, Communication::main::ConnexionDetailPrecision::LevelEnd};
                        broadCastLevelEnd.header.size = 0;
                        _GameServerTCP.SendAll(broadCastLevelEnd);
                        while (Rengine::Clock::getElapsedTime() < 0.5f) {}
                        _running = false;
                        break;
                    } else {
                        std::cout << "Scene is over" << std::endl;
                        Message<Communication::TypeDetail> broadCastSceneEnd;
                        broadCastSceneEnd.header.type = {Communication::Type::ConnexionDetail, Communication::main::ConnexionDetailPrecision::SceneFinish};
                        broadCastSceneEnd.header.size = 0;
                        _GameServerTCP.SendAll(broadCastSceneEnd);
                    }
                }

                Rengine::SparseArray<Components::Position>& positions = this->_ecs->getComponents<Components::Position>();
                Rengine::SparseArray<Components::Life>& lifes = this->_ecs->getComponents<Components::Life>();
                Rengine::SparseArray<Components::Configuration>& configurations = this->_ecs->getComponents<Components::Configuration>();
                Rengine::SparseArray<Components::Action>& actions = this->_ecs->getComponents<Components::Action>();
                Rengine::SparseArray<Components::Relationship>& relation = this->_ecs->getComponents<Components::Relationship>();

                for (std::optional<std::pair<std::shared_ptr<Connexion<Communication::TypeDetail>>, Message<Communication::TypeDetail>>> msg = _GameServerTCP.Receive(); msg; msg = _GameServerTCP.Receive()) {
                    _handleTCPMessage(msg->first, msg->second);
                }

                for (std::optional<std::pair<asio::ip::udp::endpoint, Message<Communication::TypeDetail>>> msg = _GameServerUDP.Receive(); msg; msg = _GameServerUDP.Receive()) {
                    std::shared_ptr<userGame> user = _getUserByUDPClient(msg->first);
                    if (user) {
                        _handleUDPMessage(user, msg->second, actions);
                    } else if (msg->second.header.type.type == Communication::Type::ConnexionDetail && msg->second.header.type.precision == Communication::main::ConnexionDetailPrecision::ClientConnexion) {
                        asio::ip::tcp::endpoint TCPEndpoint;
                        msg->second >> TCPEndpoint;
                        std::shared_ptr<userGame> user = _getUserByTCPEndpoint(TCPEndpoint);
                        if (user) {
                            _GameServerUDP.RemoveUser(user->user);
                            User newUser = {msg->first};
                            user->user = newUser;
                        }
                    }
                }

                // partie movement
                this->_ecs->runComponentFunction<Components::Action>(this->_ecs);  // handle action
                this->_ecs->runComponentFunction<Components::Velocity>();  // move entity

                // partie collision
                this->_ecs->runComponentFunction<Components::Hitbox>();  // handle collision

                // partie game rule
                this->_ecs->runComponentFunction<Components::Life>();  // handle life
                this->_ecs->runComponentFunction<Components::Chrono>();  // handle chrono

                std::vector<std::shared_ptr<Connexion<Communication::TypeDetail>>> clients;
                clients.swap(_GameServerTCP.getDisconnectedClients());

                for (auto it = clients.begin(); it != clients.end(); it++) {
                    std::shared_ptr<userGame> user = _getUserByClient(*it);
                    if (user) {
                        this->_GameServerUDP.RemoveUser(user->user);
                        try {
                            if (!_ecs->isEntityActive(user->entity))
                                continue;
                            Rengine::Entity &entity = this->_ecs->getEntity(user->entity);
                            this->_ecs->removeEntity<>(entity);
                            _users.erase(std::remove(_users.begin(), _users.end(), user), _users.end());
                        } catch (Rengine::ECSExceptionEntityNotFound &e) {
                            std::cerr << "[" << this->getGameName() << "]: Entity #" << user->entity << " not found. Skipping disconnection warning..." << std::endl;
                        }
                    }
                }

                for (Rengine::Entity::size_type index = 0; index <= this->_ecs->getHighestEntityId(); index++) {
                    try {
                        if (!_ecs->isEntityActive(index))
                            continue;
                        Rengine::Entity &entity = this->_ecs->getEntity(index);
                        if (lifes[index].has_value() == false || positions[index].has_value() == false || configurations[index].has_value() == false || relation[index].has_value() == false) {
                            continue;
                        }
                        Message<Communication::TypeDetail> msg;
                        msg.header.type = {Communication::Type::EntityInfo, Communication::main::EntityInfoPrecision::InfoAll};
                        msg.header.size = 0;
                        uint16_t configID = Config::EntityConfigurationIdResolverSingletone::get().get(configurations[index]->getConfig().getJsonPath());
                        // std::cout << "Entity Group => " << relation[index]->getGroup() << std::endl;
                        msg << index << lifes[index]->getHp() << lifes[index]->getMaxHp() << positions[index]->getVector2D().x << positions[index]->getVector2D().y << configID << relation[index]->getGroup();
                        _GameServerUDP.SendAll(msg);
                    } catch (Rengine::ECSExceptionEntityNotFound &e) {
                        std::cerr << "[" << this->getGameName() << "]: Entity #" << index << " not found. Ignoring its stats..." << std::endl;
                        continue;
                    }
                }

                Rengine::Clock::restart();
            } catch (std::exception &e) {
                std::cerr << "[" << this->getGameName() << "]: Exception: " << e.what() << std::endl;
            }
        }
    }

    Games::~Games() {
        std::cout << "[" << this->getGameName() << "Closing..." << std::endl;
        if (_gameThread.joinable())
            _gameThread.join();
    }
}  // namespace RType
