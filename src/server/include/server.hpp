/*
#pragma once
#include <rengine/Rengine.hpp>
#include <rengine/RengineNetwork.hpp>
#include <map>
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <stdint.h>

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
            RequestPlaybleEntity = 7,
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
            DeleteEntity = 9,
        };
    };

    struct TypeDetail {
        Type type;
        uint16_t precision;
    };
};

class Server {
    private:
        registry reg;
        EntityManager em;
        std::unique_ptr<EntityMaker> maker;
        ServerConnection<Communication::TypeDetail> server;
        bool isRunning = true;
        void handleMessage(Message<Communication::TypeDetail> &msg, const asio::ip::udp::endpoint &endpoint);
        std::unordered_map<User, Entity> users;

        System::movement movement;
        System::Collision collision;
        System::Lifetime lifetimeSystem;
        System::Script script;

    public:
        explicit Server(int32_t port);
        void destroy_entity(Entity e);
        void run();
        Entity &MakeEntity(uint16_t configurationId);
        ServerConnection<Communication::TypeDetail> &getServer() { return server; }
        std::unordered_map<User, Entity> &getUsers() { return users; }
        std::unique_ptr<EntityMaker> &getMaker() { return maker; }
        std::vector<Entity> getEntities() { return em.getActiveEntities(); }
        ~Server() = default;
};

std::string find_entity_path(uint16_t id);
*/
