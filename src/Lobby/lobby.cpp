#include "./lobby.hpp"

Lobby::Lobby(uint16_t port) : _server(port, "Lobby", false) {
    _port = port;
    _ip = _server.getIp();
    _runing = true;
}

std::shared_ptr<RType::Games> Lobby::getGameByID(Rengine::UUID::uuid_t &gameID) {
    for (auto &game : _games) {
        if (Rengine::UUID::compareUUID(game->getGameID(), gameID))
            return game;
    }
    return nullptr;
}

bool Lobby::_handleMessages(std::shared_ptr<Connexion<Communication::TypeDetail>> client, Message<Communication::TypeDetail> &msg) {
    switch (msg.header.type.precision) {
        case Communication::main::LobbyInfoPrecision::CreateGame: {
            char name[15];
            msg >> name;
            std::shared_ptr<RType::Games> newGame = createGame(_ip, (uint16_t)0, (uint16_t)0, name);
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
                char name[15];
                memset(name, 0, 15);
                strcpy(name, newGame->getGameName());
                broadCast << newGame->getGameID() << (uint16_t) newGame->getNbUsers() << name << newGame->getTimeStarted();
                std::cout << "Game with ID: " << Rengine::UUID::printUUID(newGame->getGameID()) << " and " << newGame->getNbUsers() << " users. Started at: " << newGame->getTimeStarted() << " with name: " << name << " on level " << newGame->getLevelName() << std::endl;
                _server.SendAll(broadCast, client);
            }
            break;
        }

        case Communication::main::LobbyInfoPrecision::RequestGamesInfo: {
            Message<Communication::TypeDetail> response;
            response.header.type = {Communication::Type::LobbyInfo, Communication::main::LobbyInfoPrecision::GameInfo};
            response.header.size = 0;
            for (auto &game : _games) {
                char name[15];
                memset(name, 0, 15);
                strcpy(name, game->getGameName());
                response << game->getGameID() << (uint16_t) game->getNbUsers() << name << game->getTimeStarted();
            }
            uint16_t nbGames = _games.size();
            response << nbGames;
            _server.Send(response, client);
            break;
        }

        case Communication::main::LobbyInfoPrecision::JoinGame: {
            Rengine::UUID::uuid_t gameID;
            msg >> gameID;
            std::shared_ptr<RType::Games> game = getGameByID(gameID);
            if (game != nullptr) {
                Message<Communication::TypeDetail> response;
                response.header.type = {Communication::Type::LobbyInfo, Communication::main::LobbyInfoPrecision::RequestedGame};
                response.header.size = 0;
                uint16_t gameTCPport = game->getTCPPort();
                uint16_t gameUDPport = game->getUDPPort();
                response << gameTCPport << gameUDPport;
                _server.Send(response, client);
                Message<Communication::TypeDetail> broadCast;
                broadCast.header.type = {Communication::Type::LobbyInfo, Communication::main::LobbyInfoPrecision::GameUpdated};
                broadCast.header.size = 0;
                char name[15];
                memset(name, 0, 15);
                strcpy(name, game->getGameName());
                broadCast << game->getGameID() << (uint16_t) game->getNbUsers() << name << game->getTimeStarted();
                _server.SendAll(broadCast, client);
            }
            break;
        }
    }
    return true;
}

void Lobby::run() {
    while (_runing) {
        try {
            for (std::optional<std::pair<std::shared_ptr<Connexion<Communication::TypeDetail>>, Message<Communication::TypeDetail>>> msg = _server.Receive(); msg; msg = _server.Receive()) {
                if (msg->second.header.type.type == Communication::Type::LobbyInfo)
                    _handleMessages(msg->first, msg->second);
            }
            for (auto &it : _games) {
                if (!it->isRunning()) {
                    std::cout << "[Lobby] Game with ID: " << Rengine::UUID::printUUID(it->getGameID()) << " is over." << std::endl;
                    _games.erase(std::remove(_games.begin(), _games.end(), it), _games.end());
                }
            }
        } catch (std::exception &e) {
            std::cerr << "[Lobby] Error: " << e.what() << std::endl;
        }
    }
}

size_t Lobby::getNbGames() {
    return _games.size();
}

std::shared_ptr<RType::Games> Lobby::createGame(std::string &ip, uint16_t UDPPort, uint16_t TCPPort, char (&name)[15]) {
    std::shared_ptr<RType::Games> newGame = std::make_shared<RType::Games>(ip, UDPPort, TCPPort, name);

    newGame->start();
    _games.push_back(newGame);
    return newGame;
}

Lobby::~Lobby() {
    _runing = false;
    try {
        for (auto &game : _games) {
            if (game->isRunning())
                game->stop();
        }
    } catch (std::exception &e) {
        std::cerr << "[Lobby] Error: " << e.what() << std::endl;
    }
}
