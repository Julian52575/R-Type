#include "./game.hpp"
#include "src/Components/Velocity.hpp"
#include "src/Components/Hitbox.hpp"
#include "src/Components/Life.hpp"
#include "src/Components/Chrono.hpp"
#include "src/Components/Metadata.hpp"
#include "src/Components/Buff.hpp"
#include "src/Config/EntityConfigResolver.hpp"
#include "src/Config/ConfigurationIdResolver.hpp"

RType::Games::Games(std::string &ip, uint16_t TCPPort, uint16_t UDPPort, std::string gameName) : _GameServerTCP(TCPPort, gameName), _GameServerUDP(UDPPort, gameName), _levelManager(nullptr) {
    Rengine::UUID::generateUUID(_gameID);
    _ecs = std::make_shared<Rengine::ECS>();
    _levelManager.setECS(_ecs);
    _TCPPort = _GameServerTCP.getPort();
    _UDPPort = _GameServerUDP.getPort();
    _gameName = gameName;
    _ip = ip;
    _timeStarted = time(NULL);
    _running = false;
};

void RType::Games::start() {
    this->_ecs->registerComponent<RType::Components::Configuration>();
    this->_ecs->registerComponent<RType::Components::Position>();
    this->_ecs->registerComponent<Components::Buff>();
    this->_ecs->registerComponent<RType::Components::Hitbox>();
    this->_ecs->registerComponent<RType::Components::Relationship>();
    this->_ecs->registerComponent<RType::Components::Metadata>();
    this->_ecs->registerComponent<RType::Components::Velocity>();
    this->_ecs->registerComponent<RType::Components::Chrono>();
    this->_ecs->registerComponent<RType::Components::Life>();

    // Function
    this->_ecs->setComponentFunction<RType::Components::Velocity>(RType::Components::Velocity::componentFunction);
    this->_ecs->setComponentFunction<RType::Components::Hitbox>(RType::Components::Hitbox::componentFunction);
    this->_ecs->setComponentFunction<RType::Components::Chrono>(RType::Components::Chrono::componentFunction);
    this->_ecs->setComponentFunction<RType::Components::Life>(RType::Components::Life::componentFunction);
    this->_levelManager.loadLevel("assets/levels/template.json");
    _timeStarted = time(NULL);
    _running = true;
    _gameThread = std::thread(&RType::Games::run, this);
};

void RType::Games::stop() {
    _running = false;
    _gameThread.join();
};

time_t &RType::Games::getTimeStarted() {
    return _timeStarted;
};

uuid_t &RType::Games::getGameID() {
    return _gameID;
};

std::string &RType::Games::getIP() {
    return _ip;
};

uint16_t &RType::Games::getTCPPort() {
    return _TCPPort;
};

uint16_t &RType::Games::getUDPPort() {
    return _UDPPort;
};

size_t RType::Games::getNbUsers() {
    return _users.size();
};

const char *RType::Games::getGameName() {
    return _gameName.c_str();
};

std::shared_ptr<userGame> RType::Games::_getUserByClient(std::shared_ptr<Connexion<Communication::TypeDetail>> client) {
    for (std::shared_ptr<userGame> user : _users) {
        if (user->client == client)
            return user;
    }
    return nullptr;
};

std::shared_ptr<userGame> RType::Games::_getUserByEntityID(uint16_t entityID) {
    for (std::shared_ptr<userGame> user : _users) {
        if (user->entityID == entityID)
            return user;
    }
    return nullptr;
};

std::shared_ptr<userGame> RType::Games::_getUserByUDPClient(asio::ip::udp::endpoint &UDPClient) {
    for (std::shared_ptr<userGame> user : _users) {
        if (user->user.endpoint == UDPClient)
            return user;
    }
    return nullptr;
};

void RType::Games::run() {
    while (_running) {
        for (std::optional<std::pair<std::shared_ptr<Connexion<Communication::TypeDetail>>, Message<Communication::TypeDetail>>> msg = _GameServerTCP.Receive(); msg; msg = _GameServerTCP.Receive()) {
            std::cout << "Received message from TCPclient: " << msg->first->getSocket().remote_endpoint() << " with size: " << msg->second.header.size << std::endl;
            _handleTCPMessage(msg->first, msg->second);
        }

        for (std::optional<std::pair<asio::ip::udp::endpoint, Message<Communication::TypeDetail>>> msg = _GameServerUDP.Receive(); msg; msg = _GameServerUDP.Receive()) {
            std::cout << "Received message from UDPclient: " << msg->first << " with size: " << msg->second.header.size << std::endl;
            std::shared_ptr<userGame> user = _getUserByUDPClient(msg->first);
            if (user) {
                std::cout << "User found with entityID: " << user->entityID << std::endl;
            }
        }

        this->_levelManager.updateDeltatime();
        if (this->_levelManager.isCurrentSceneOver()) {
            if (!this->_levelManager.nextScene()) {
                //Level is over
            }
        }
        //partie movement
        // this->_ecs->runComponentFunction<RType::Components::Action>();  // handle action player
        this->_ecs->runComponentFunction<RType::Components::Velocity>();  // move entity

        //partie collision
        this->_ecs->runComponentFunction<RType::Components::Hitbox>();  // handle collision

        //partie game rule
        this->_ecs->runComponentFunction<RType::Components::Life>();  // handle life
        this->_ecs->runComponentFunction<RType::Components::Chrono>();  // handle chrono

        Rengine::SparseArray<RType::Components::Position>& positions = this->_ecs->getComponents<RType::Components::Position>();
        Rengine::SparseArray<RType::Components::Life>& lifes = this->_ecs->getComponents<RType::Components::Life>();
        Rengine::SparseArray<RType::Components::Configuration>& configurations = this->_ecs->getComponents<RType::Components::Configuration>();

        if (Rengine::Clock::getElapsedTime() > 0.05) {
            for (Rengine::Entity::size_type index = 0; index <= this->_ecs->getHighestEntityId(); index++) {
                Rengine::Entity &entity = this->_ecs->getEntity(index);
                if (lifes[index].has_value() == false || positions[index].has_value() == false) {
                    continue;
                }
                Message<Communication::TypeDetail> msg;
                msg.header.type = {Communication::Type::EntityInfo, Communication::main::EntityInfoPrecision::InfoAll};
                msg.header.size = 0;
                uint16_t configID = RType::Config::EntityConfigurationIdResolverSingletone::get().get(configurations[index]->getConfig().getJsonPath());
                msg << index << lifes[index]->getHp() << lifes[index]->getMaxHp() << positions[index]->getVector2D().x << positions[index]->getVector2D().y << configID;
                _GameServerUDP.SendAll(msg);
            }
        }

        std::vector<std::shared_ptr<Connexion<Communication::TypeDetail>>> clients;
        clients.swap(_GameServerTCP.getDisconnectedClients());

        for (auto it = clients.begin(); it != clients.end(); it++) {
            std::shared_ptr<userGame> user = _getUserByClient(*it);
            if (user) {
                this->_GameServerUDP.RemoveUser(user->user);
                Rengine::Entity &entity = this->_ecs->getEntity(user->entity);
                Message<Communication::TypeDetail> msg;
                msg.header.type = {Communication::Type::ConnexionDetail, Communication::main::ConnexionDetailPrecision::ClientDisconnect};
                msg.header.size = 0;
                Rengine::Entity::size_type id = entity;
                msg << id;
                _GameServerUDP.SendAll(msg);
                _users.erase(std::remove(_users.begin(), _users.end(), user), _users.end());
            }
        }
    }
};

RType::Games::~Games() {
    if (_gameThread.joinable())
        _gameThread.join();
};
