#pragma once
#include <rengine/Rengine.hpp>
#include <rengine/src/Core/Rengine.hpp>
#include <rengine/RengineNetwork.hpp>
#include <unordered_map>
#include <memory>
#include "include/utils.hpp"

struct Communication {
    enum Type : uint16_t {
        NOOP = 0,
        ConnexionDetail = 1,
        JsonDetail = 2,
        EntityAction = 3,
        EntityInfo = 4,
    };

    union main {
        enum NOOPPrecision : uint16_t {
            NOOP = 0,
        };

        enum ConnexionDetailPrecision : uint16_t {
            ClientConnexion = 0,
            ClientDisconnect = 1,
            ServerAccept = 2,
            ServerDisconnect = 3,
            ClientReady = 4,
            PlayableEntityInGameId = 5,
            ServerStop = 6,
        };

        enum JsonDetailPrecision : uint16_t {
            GamePartLevel = 0,
            GamePartEntity = 1,
            GamePartPlugin = 2,
        };

        enum EntityActionPrecision : uint16_t {
            Move = 0,
            Shoot1 = 1,
            Shoot2 = 2,
            Shoot3 = 3,
            Dodge = 4,
            Barrier = 5,
            Death = 6,
            Ultimate = 7
        };

        enum EntityInfoPrecision : uint16_t {
            InfoAll = 0,
            InGameAndConfigurationId = 1,
            InfoPosition = 2,
            InfoHP = 3,
            InfoMaxHp = 4,
            InfoSpeed = 5,
            InfoMaxSpeed = 6,
            InfoDirection = 7,
            NewEntity = 8,
        };
    };

    struct TypeDetail {
        Type type;
        uint16_t precision;
    };
};

class Game {
    public:
        Game(std::string ip, uint16_t port);
        void handleMessage(Message<Communication::TypeDetail> &msg);
        std::unordered_map<uint16_t, Entity> &getEntities();
        Rengine::Rengine &getCore();
        ClientConnection<Communication::TypeDetail> &getClient();
        std::shared_ptr<Entity> getPlayer();
        void setPlayer(Entity &player);
        ~Game();

    private:
        ClientConnection<Communication::TypeDetail> client;
        std::unordered_map<uint16_t, Entity> entities;
        Rengine::Rengine core;
        std::shared_ptr<Entity> player;
};
