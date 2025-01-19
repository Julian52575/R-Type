//
#pragma once
#include <string>
#include <memory>
#include <rengine/RengineGraphics.hpp>
#include <rengine/Rengine.hpp>

namespace RType {

    struct NetworkInfo {
        std::string ip = "0.0.0.0";
        uint16_t TCPPort = 0;
        uint16_t UDPPort = 0;
        std::string lobbyName = "";
    };

    struct LobbyInfo {
        std::string serverIp = "127.0.0.1";
        uint16_t port = 4242;
        std::string playerJson = "assets/entities/spaceShip.json";
    };

    struct GameInfo {
        std::string name;
        Rengine::UUID::uuid_t id;
        uint16_t playerCount;
        time_t time;
    };

    struct DisplayGameInfo {
        std::shared_ptr<Rengine::Graphics::AText> name;
        std::shared_ptr<Rengine::Graphics::AText> playerCount;
        std::shared_ptr<Rengine::Graphics::AText> time;
        std::shared_ptr<Rengine::Graphics::ASprite> background;
        GameInfo Infos;
    };

}  // namespace RType
