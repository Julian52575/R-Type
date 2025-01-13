#include <array>
#include <string>
#include <sstream>
#include "./ServerConnexionUDP.hpp"
#include "./ServerConnexionTCP.hpp"

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
        EntityInfo = 4        ///< Information about entities (e.g., position, health).
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
         * @brief Enum for precision of JSON-related details.
         */
        enum JsonDetailPrecision : uint16_t {
            GamePartLevel = 0,   ///< Details about game levels.
            GamePartEntity = 1,  ///< Details about game entities.
            GamePartPlugin = 2   ///< Details about game plugins.
        };

        /**
         * @brief Enum for precision of entity action types.
         */
        enum EntityActionPrecision : uint16_t {
            Move = 0,      ///< Movement action.
            Shoot1 = 1,    ///< Basic shooting action.
            Shoot2 = 2,    ///< Secondary shooting action.
            Shoot3 = 3,    ///< Tertiary shooting action.
            Dodge = 4,     ///< Dodge action.
            Barrier = 5,   ///< Activate a barrier.
            Death = 6,     ///< Death action.
            Ultimate = 7   ///< Perform an ultimate move.
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
    };

    /**
     * @brief Structure containing details about the communication type and precision.
     */
    struct TypeDetail {
        Type type;          ///< The main communication type.
        uint16_t precision;  ///< The precision level for the communication type.
    };
};

std::string IntToIp(const std::array<uint16_t, 4>& ipInt) {
    std::ostringstream ss;

    for (size_t i = 0; i < ipInt.size(); ++i) {
        ss << ipInt[i];
        if (i < ipInt.size() - 1) {
            ss << '.';
        }
    }

    return ss.str();
}

class test {
    public:
        ServerUDP<Communication::TypeDetail> serverUDP;
        ServerTCP<Communication::TypeDetail> serverTCP;

        test(uint16_t port) : serverUDP(port), serverTCP(port + 1) {};
        ~test() {};
};

int main(int ac, char **argv) {
    uint16_t port = std::stoi(argv[1]);
    uint16_t port2 = port + 1;

    try {
        test server(port);

        while (true) {
            if (auto msg = server.serverUDP.Receive()) {
                std::cout << "Received message from client: " << msg->first << " with size: " << msg->second.header.size << std::endl;
            }
            if (auto msg = server.serverTCP.Receive()) {
                std::cout << "Received message from client: " << msg->first->getSocket().remote_endpoint() << " with size: " << msg->second.header.size << std::endl;
            }
        }
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }

    return 0;
}