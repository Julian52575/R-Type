#pragma once
#include <rengine/RengineNetworkServer.hpp>
#include <uuid/uuid.h>

/**
 * @brief Namespace for communication-related definitions and structures.
 * 
 * This structure defines the communication types and their associated details,
 * including types, precision values, and details about message handling.
 */
struct Communication {
    /**
     * @brief Enum for high-level communication types.
     */
    enum Type : uint16_t {
        NOOP = 0,             ///< No operation (default type).
        ConnexionDetail = 1,  ///< Details about client/server connection events.
        JsonDetail = 2,       ///< JSON-related details, such as game state or configuration.
        EntityAction = 3,     ///< Actions performed by entities (e.g., move, shoot).
        EntityInfo = 4,       ///< Information about entities (e.g., position, health).
        LobbyInfo = 5         ///< Information about the lobby.
    };

    /**
     * @brief Union containing enums for specific precision types of communication messages.
     */
    union main {
        /**
         * @brief Enum for precision of NOOP messages.
         */
        enum NOOPPrecision : uint16_t {
            NOOP = 0  ///< No operation.
        };

        /**
         * @brief Enum for precision of connection-related messages.
         */
        enum ConnexionDetailPrecision : uint16_t {
            ClientConnexion = 0,           ///< A client has connected.
            ClientDisconnect = 1,          ///< A client has disconnected.
            ServerAccept = 2,              ///< The server has accepted a connection.
            ServerDisconnect = 3,          ///< The server has disconnected a client.
            ClientReady = 4,               ///< A client is ready.
            PlayableEntityInGameId = 5,    ///< The ID of a playable entity in the game.
            ServerStop = 6,                ///< The server has stopped.
            RequestPlaybleEntity = 7       ///< Request for a playable entity.
        };

        /**
         * @brief Enum for precision of entity action types.
         */
        enum EntityActionPrecision : uint16_t {
            Move = 0,      ///< Movement action.
            Shoot = 1,     ///< Basic shooting action.
            Dodge = 2,     ///< Dodge action.
            Barrier = 3,   ///< Activate a barrier.
            Death = 4,     ///< Death action.
            Ultimate = 5   ///< Perform an ultimate move.
        };

        /**
         * @brief Enum for precision of entity information types.
         */
        enum EntityInfoPrecision : uint16_t {
            InfoAll = 0,              ///< All information about an entity.
            InGameAndConfigurationId = 1,  ///< In-game and configuration ID of an entity.
            InfoPosition = 2,         ///< Position information of an entity.
            InfoHP = 3,               ///< Current health points of an entity.
            InfoMaxHp = 4,            ///< Maximum health points of an entity.
            InfoSpeed = 5,            ///< Current speed of an entity.
            InfoMaxSpeed = 6,         ///< Maximum speed of an entity.
            InfoDirection = 7,        ///< Direction of an entity.
            NewEntity = 8,            ///< A new entity has been created.
            DeleteEntity = 9          ///< An entity has been deleted.
        };

        enum LobbyInfoPrecision : uint16_t {
            GameCreated = 0,  ///< A new game has been created.
            GameDeleted = 1,  ///< A game has been deleted.
            GameUpdated = 2,     ///< A client has joined a game.
            CreateGame = 3,
            RequestedGame = 4,
            GameInfo = 5,
            RequestGamesInfo = 6,
            JoinGame = 7
        };
    };

    /**
     * @brief Structure containing details about the communication type and precision.
     */
    struct TypeDetail {
        Type type;          ///< The main communication type.
        uint16_t precision;  ///< The precision level for the communication type.
    };
};

struct userGame {
    std::shared_ptr<Connexion<Communication::TypeDetail>> client;
    asio::ip::udp::endpoint UDPClient;
    uint16_t entityID;
};

class Games {
    public:
        Games(std::string &ip, uint16_t TCPPort, uint16_t UDPPort, std::string gameName);
        void start();
        void stop();
        time_t &getTimeStarted();
        uuid_t &getGameID();
        std::string &getIP();
        uint16_t &getTCPPort();
        uint16_t &getUDPPort();
        size_t getNbUsers();
        void run();
        ~Games();

    private:
        void _handleTCPMessage(std::shared_ptr<Connexion<Communication::TypeDetail>> client, Message<Communication::TypeDetail> &msg);
        void _handleConnexionTCPMessage(std::shared_ptr<Connexion<Communication::TypeDetail>> client, Message<Communication::TypeDetail> &msg);

        std::shared_ptr<userGame> _getUserByClient(std::shared_ptr<Connexion<Communication::TypeDetail>> client);
        std::shared_ptr<userGame> _getUserByEntityID(uint16_t entityID);
        std::shared_ptr<userGame> _getUserByUDPClient(asio::ip::udp::endpoint &UDPClient);
        std::thread _gameThread;
        bool _running;
        uuid_t _gameID;
        std::string _ip;
        uint16_t _TCPPort;
        uint16_t _UDPPort;
        time_t _timeStarted;
        ServerTCP<Communication::TypeDetail> _GameServerTCP;
        ServerUDP<Communication::TypeDetail> _GameServerUDP;
        std::vector<std::shared_ptr<userGame>> _users;
};
