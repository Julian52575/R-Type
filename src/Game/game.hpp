#pragma once
#include <rengine/RengineNetworkServer.hpp>
#include <rengine/Rengine.hpp>
#include "LevelManager.hpp"
#include "src/Components/Action.hpp"
#include <string>
#include <vector>
#include <memory>

struct Communication {
    enum Type : uint16_t {
        NOOP = 0,
        ConnexionDetail = 1,
        JsonDetail = 2,
        EntityAction = 3,
        EntityInfo = 4,
        LobbyInfo = 5
    };
    union main {
        enum NOOPPrecision : uint16_t {
            NOOP = 0
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
            LevelEnd = 8,
            SceneFinish = 9
        };
        enum EntityActionPrecision : uint16_t {
            EntityActionTypeMove   = 0x00,
            EntityActionTypeShoot1 = 0x01,
            EntityActionTypeShoot2 = 0x02,
            EntityActionTypeShoot3 = 0x03,
            EntityActionTypeDodge  = 0x04,
            EntityActionTypeBarrier = 0x05,
            EntityActionTypeDeath  = 0x06,
            EntityActionTypeUltimate = 0x07
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
            DeleteEntity = 9
        };
        enum LobbyInfoPrecision : uint16_t {
            GameCreated = 0,
            GameDeleted = 1,
            GameUpdated = 2,
            CreateGame = 3,
            RequestedGame = 4,
            GameInfo = 5,
            RequestGamesInfo = 6,
            JoinGame = 7
        };
    };
    struct TypeDetail {
        Type type;
        uint16_t precision;
    };
};

struct userGame {
    std::shared_ptr<Connexion<Communication::TypeDetail>> client;
    Rengine::Entity::size_type entity;
};
namespace RType {
class Games {
    public:
        Games(std::string &ip, uint16_t TCPPort, uint16_t UDPPort, std::string gameName);
        bool isRunning();
        void start();
        void stop();
        time_t &getTimeStarted();
        Rengine::UUID::uuid_t &getGameID();
        std::string &getIP();
        uint16_t &getTCPPort();
        uint16_t &getUDPPort();
        size_t getNbUsers();
        const char *getGameName();
        void run();
        ~Games();
        const std::string& getLevelName(void) const noexcept;

    private:
        void initLevel(void);
        void _handleTCPMessage(std::shared_ptr<Connexion<Communication::TypeDetail>> client, Message<Communication::TypeDetail> &msg, Rengine::SparseArray<RType::Components::Action>& actions);
        void _handleConnexionTCPMessage(std::shared_ptr<Connexion<Communication::TypeDetail>> client, Message<Communication::TypeDetail> &msg);
        void _handleEntityInfoUDPMessage(std::shared_ptr<Connexion<Communication::TypeDetail>> client, Message<Communication::TypeDetail> &msg, Rengine::SparseArray<RType::Components::Action>& actions);
        std::shared_ptr<userGame> _getUserByClient(std::shared_ptr<Connexion<Communication::TypeDetail>> client);
        std::shared_ptr<userGame> _getUserByTCPEndpoint(asio::ip::tcp::endpoint &TCPEndpoint);
        std::shared_ptr<userGame> _getUserByEntity(Rengine::Entity::size_type entity);
        std::thread _gameThread;
        bool _running;
        Rengine::UUID::uuid_t _gameID;
        std::string _gameName;
        std::string _ip;
        uint16_t _TCPPort;
        uint16_t _UDPPort;
        time_t _timeStarted;
        ServerTCP<Communication::TypeDetail> _GameServerTCP;
        ServerUDP<Communication::TypeDetail> _GameServerUDP;
        std::vector<std::shared_ptr<userGame>> _users;
        std::shared_ptr<Rengine::ECS> _ecs;
        LevelManager _levelManager;
};
}  // namespace RType
