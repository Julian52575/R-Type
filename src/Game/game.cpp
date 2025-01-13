#include "./game.hpp"

Games::Games(std::string &ip, uint16_t TCPPort, uint16_t UDPPort, std::string gameName) : _GameServerTCP(TCPPort, gameName), _GameServerUDP(UDPPort, gameName) {
    uuid_generate_random(_gameID);
    _TCPPort = _GameServerTCP.getPort();
    _UDPPort = _GameServerUDP.getPort();
    _ip = ip;
    _timeStarted = time(NULL);
};

void Games::start() {
    _gameThread = std::thread(&Games::run, this);
    _timeStarted = time(NULL);
    _running = true;
};

void Games::stop() {
    _running = false;
    _gameThread.join();
};

time_t &Games::getTimeStarted() {
    return _timeStarted;
};

uuid_t &Games::getGameID() {
    return _gameID;
};

std::string &Games::getIP() {
    return _ip;
};

uint16_t &Games::getTCPPort() {
    return _TCPPort;
};

uint16_t &Games::getUDPPort() {
    return _UDPPort;
};

size_t Games::getNbUsers() {
    return _users.size();
};

std::shared_ptr<userGame> Games::_getUserByClient(std::shared_ptr<Connexion<Communication::TypeDetail>> client) {
    for (std::shared_ptr<userGame> user : _users) {
        if (user->client == client)
            return user;
    }
    return nullptr;
};

std::shared_ptr<userGame> Games::_getUserByEntityID(uint16_t entityID) {
    for (std::shared_ptr<userGame> user : _users) {
        if (user->entityID == entityID)
            return user;
    }
    return nullptr;
};

std::shared_ptr<userGame> Games::_getUserByUDPClient(asio::ip::udp::endpoint &UDPClient) {
    for (std::shared_ptr<userGame> user : _users) {
        if (user->UDPClient == UDPClient)
            return user;
    }
    return nullptr;
};

void Games::run() {
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

        std::vector<std::shared_ptr<Connexion<Communication::TypeDetail>>> clients;
        clients.swap(_GameServerTCP.getDisconnectedClients());

        for (auto it = clients.begin(); it != clients.end(); it++) {
            std::shared_ptr<userGame> user = _getUserByClient(*it);
            if (user) {
                _users.erase(std::remove(_users.begin(), _users.end(), user), _users.end());
            }
        }
    }
};

Games::~Games() {
    if (_gameThread.joinable())
        _gameThread.join();
};
