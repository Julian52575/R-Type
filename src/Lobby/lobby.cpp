#include "./lobby.hpp"

Lobby::Lobby(uint16_t port) : _server(port, "Lobby", false) {
    _port = port;
    _ip = _server.getIp();
    _runing = true;
};

std::string Lobby::printUuid(uuid_t &uuid) {
    char uuid_str[37];
    uuid_unparse(uuid, uuid_str);
    return std::string(uuid_str);
};

std::shared_ptr<Games> Lobby::getGameByID(uuid_t &gameID) {
    for (auto &game : _games) {
        if (uuid_compare(game->getGameID(), gameID) == 0)
            return game;
    }
    return nullptr;
};

bool Lobby::_handleMessages(std::shared_ptr<Connexion<Communication::TypeDetail>> client, Message<Communication::TypeDetail> &msg) {
    switch (msg.header.type.precision) {
        case Communication::main::LobbyInfoPrecision::CreateGame: {
            char name[15];
            msg >> name;
            std::shared_ptr<Games> newGame = createGame(_ip, (uint16_t)0, (uint16_t)0, name);
            if (newGame != nullptr) {
                Message<Communication::TypeDetail> response;
                response.header.type = {Communication::Type::LobbyInfo, Communication::main::LobbyInfoPrecision::RequestedGame};
                response.header.size = 0;
                uint16_t gameTCPport = newGame->getTCPPort();
                uint16_t gameUDPport = newGame->getUDPPort();
                response << gameTCPport << gameUDPport;
                _server.Send(response, client);
                Message<Communication::TypeDetail> broadCast;
                broadCast.header.type = {Communication::Type::LobbyInfo, Communication::main::LobbyInfoPrecision::GameCreated};
                broadCast.header.size = 0;
                broadCast << newGame->getGameID() << (uint16_t) newGame->getNbUsers();
                _server.SendAll(broadCast, client);
            }
            break;
        }

        case Communication::main::LobbyInfoPrecision::RequestGamesInfo: {
            Message<Communication::TypeDetail> response;
            response.header.type = {Communication::Type::LobbyInfo, Communication::main::LobbyInfoPrecision::GameInfo};
            response.header.size = 0;
            for (auto &game : _games) {
                response << game->getGameID() << (uint16_t) game->getNbUsers();
            }
            uint16_t nbGames = _games.size();
            response << nbGames;
            _server.Send(response, client);
            break;
        }

        case Communication::main::LobbyInfoPrecision::JoinGame: {
            uuid_t gameID;
            msg >> gameID;
            std::shared_ptr<Games> game = getGameByID(gameID);
            if (game != nullptr) {
                Message<Communication::TypeDetail> response;
                response.header.type = {Communication::Type::LobbyInfo, Communication::main::LobbyInfoPrecision::RequestedGame};
                response.header.size = 0;
                uint16_t gameTCPport = game->getTCPPort();
                uint16_t gameUDPport = game->getUDPPort();
                response << gameTCPport << gameUDPport;
                _server.Send(response, client);
            }
            break;
        }
            
    }
    return true;
};

void Lobby::run() {
    while (_runing) {
        for (std::optional<std::pair<std::shared_ptr<Connexion<Communication::TypeDetail>>, Message<Communication::TypeDetail>>> msg = _server.Receive(); msg; msg = _server.Receive()) {
            std::cout << "Received message from client: " << msg->first->getSocket().remote_endpoint() << " with size: " << msg->second.header.size << std::endl;
            if (msg->second.header.type.type == Communication::Type::LobbyInfo)
                _handleMessages(msg->first, msg->second);
        }
    }
};

size_t Lobby::getNbGames() {
    return _games.size();
};

std::shared_ptr<Games> Lobby::createGame(std::string &ip, uint16_t UDPPort, uint16_t TCPPort, char (&name)[15]) {
    std::shared_ptr<Games> newGame = std::make_shared<Games>(ip, UDPPort, TCPPort, name);
    newGame->start();
    _games.push_back(newGame);
    return newGame;
};

Lobby::~Lobby() {
    _runing = false;
    for (auto &game : _games) {
        game->stop();
    }
};
