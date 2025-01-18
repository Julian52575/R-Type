#include <rengine/RengineNetworkClient.hpp>
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

std::string printUuid(Rengine::UUID::uuid_t &uuid) {
    char uuid_str[37];
    uuid_unparse(uuid, uuid_str);
    return std::string(uuid_str);
};

void game(std::string &ip, uint16_t TCPPort, uint16_t UDPPort) {
    std::cout << "Game started with TCP port: " << TCPPort << " and UDP port: " << UDPPort << std::endl;
    ClientUDP<Communication::TypeDetail> clientUDP(ip, UDPPort);
    ClientTCP<Communication::TypeDetail> clientTCP(ip, TCPPort);
    Message<Communication::TypeDetail> msg;
    msg.header.type = {Communication::Type::ConnexionDetail, Communication::main::ConnexionDetailPrecision::ClientConnexion};
    msg.header.size = 0;
    msg << clientUDP.getLocalEndpoint();
    clientTCP.Send(msg);

    while (true) {
        for (std::optional<Message<Communication::TypeDetail>> msg = clientTCP.Receive(); msg; msg = clientTCP.Receive()) {
            std::cout << "Received message from server with size: " << msg->header.size << std::endl;
        }
        for (std::optional<Message<Communication::TypeDetail>> msg = clientUDP.Receive(); msg; msg = clientUDP.Receive()) {
            std::cout << "Received message from server with size: " << msg->header.size << std::endl;
            if (msg->header.type.type == Communication::Type::EntityInfo && msg->header.type.precision == Communication::main::EntityInfoPrecision::InfoAll) {
                uint16_t health;
                uint16_t maxHealth;
                uint64_t id;
                uint16_t configID;
                float posX;
                float posY;
                *msg >> configID >> posY >> posX >> maxHealth >> health >> id;
            }
        }
    }
}

void lobby(int ac, char **argv) {
    std::string ip = argv[1];
    uint16_t TCPPort = std::stoi(argv[2]);
    ClientTCP<Communication::TypeDetail> clientTCP(ip, TCPPort);
    Message<Communication::TypeDetail> msg;
    msg.header.type = {Communication::Type::ConnexionDetail, Communication::main::ConnexionDetailPrecision::ClientConnexion};
    msg.header.size = 0;
    clientTCP.Send(msg);

    std::string input;
    while (std::getline(std::cin, input)) {
        if (input.empty()) break;
        if (input == "send") {
            Message<Communication::TypeDetail> msg2;
            msg2.header.type = {Communication::Type::EntityInfo, Communication::main::EntityInfoPrecision::InfoAll};
            msg2.header.size = 0;
            clientTCP.Send(msg2);
            std::cout << "Sent message: " << input << std::endl;
        }
        if (input == "create") {
            Message<Communication::TypeDetail> msg3;
            msg3.header.type = {Communication::Type::LobbyInfo, Communication::main::LobbyInfoPrecision::CreateGame};
            msg3.header.size = 0;
            char name[15] = "caca";
            msg3 << name;
            clientTCP.Send(msg3);
            std::cout << "Sent message: " << input << std::endl;
        }
        if (input == "request") {
            Message<Communication::TypeDetail> msg4;
            msg4.header.type = {Communication::Type::LobbyInfo, Communication::main::LobbyInfoPrecision::RequestGamesInfo};
            msg4.header.size = 0;
            clientTCP.Send(msg4);
            std::cout << "Sent message: " << input << std::endl;
        }
        if (input.find("join") != std::string::npos) {
            Message<Communication::TypeDetail> msg5;
            msg5.header.type = {Communication::Type::LobbyInfo, Communication::main::LobbyInfoPrecision::JoinGame};
            msg5.header.size = 0;
            Rengine::UUID::uuid_t gameID;
            uuid_parse(input.substr(5).c_str(), gameID);
            std::cout << "Game ID: " << printUuid(gameID) << std::endl;
            msg5 << gameID;
            clientTCP.Send(msg5);
            std::cout << "Sent message: " << input << std::endl;
        }
        if (input == "receive") {
            for (std::optional<Message<Communication::TypeDetail>> msg = clientTCP.Receive(); msg; msg = clientTCP.Receive()) {
                std::cout << "Received message from server with size: " << msg->header.size << std::endl;
                if (msg->header.type.type == Communication::Type::LobbyInfo && msg->header.type.precision == Communication::main::LobbyInfoPrecision::RequestedGame) {
                    uint16_t gameTCPport;
                    uint16_t gameUDPport;
                    *msg >> gameUDPport >> gameTCPport;
                    game(ip, gameTCPport, gameUDPport);
                    return;
                }
                if (msg->header.type.type == Communication::Type::LobbyInfo && msg->header.type.precision == Communication::main::LobbyInfoPrecision::GameUpdated) {
                    Rengine::UUID::uuid_t gameID;
                    uint16_t nbUsers;
                    char name[15];
                    time_t timeStarted;
                    *msg >> timeStarted >> name >> nbUsers >> gameID;
                    std::cout << "Game with ID: " << printUuid(gameID) << " and " << nbUsers << " users. Started at: " << timeStarted << " with name: " << name << std::endl;
                }
                if (msg->header.type.type == Communication::Type::LobbyInfo && msg->header.type.precision == Communication::main::LobbyInfoPrecision::GameInfo) {
                    uint16_t nbGames;
                    *msg >> nbGames;
                    std::cout << "Received " << nbGames << " games." << std::endl;
                    for (uint16_t i = 0; i < nbGames; i++) {
                        Rengine::UUID::uuid_t gameID;
                        uint16_t nbUsers;
                        char name[15];
                        time_t timeStarted;
                        *msg >> timeStarted >> name >> nbUsers >> gameID;
                        std::cout << "Game with ID: " << printUuid(gameID) << " and " << nbUsers << " users. Started at: " << timeStarted << " with name: " << name << std::endl;
                    }
                }
                if (msg->header.type.type == Communication::Type::LobbyInfo && msg->header.type.precision == Communication::main::LobbyInfoPrecision::GameCreated) {
                    Rengine::UUID::uuid_t gameID;
                    uint16_t nbUsers;
                    char name[15];
                    time_t timeStarted;
                    *msg >> timeStarted >> name >> nbUsers >> gameID;
                    std::cout << "Game with ID: " << printUuid(gameID) << " and " << nbUsers << " users. Started at: " << timeStarted << " with name: " << name << std::endl;
                }
            }
        }
        input.clear();
    }
}

int main (int argc, char **argv) {
    try {
        lobby(argc, argv);
    } catch(const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 84;
    }
}

