#pragma once
#include "../Game/game.hpp"

class Lobby {
    public:
        Lobby(uint16_t port);
        void run();
        void setRunning(bool runing) { _runing = runing; };
        ~Lobby();
        size_t getNbGames();

    private:
        std::string _ip;
        uint16_t _port;
        bool _runing;
        bool _handleMessages(std::shared_ptr<Connexion<Communication::TypeDetail>> client, Message<Communication::TypeDetail> &msg);
        std::shared_ptr<RType::Games> getGameByID(uuid_t &gameID);
        std::shared_ptr<RType::Games> createGame(std::string &ip, uint16_t UDPPort, uint16_t TCPPort, char (&name)[15]);
        ServerTCP<Communication::TypeDetail> _server;
        std::vector<std::shared_ptr<RType::Games>> _games;
};
