#include <iostream>
#include <string>
#include <stdint.h>
#include <sstream>
#include "./ClientConnexionUDP.hpp"
#include "./ClientConnexionTCP.hpp"

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

#include <stdexcept>

std::array<uint16_t, 4> IpToInt(const std::string& ip) {
    std::array<uint16_t, 4> ipInt;
    std::istringstream ss(ip);
    std::string ipPart;
    int i = 0;

    while (std::getline(ss, ipPart, '.') && i < 4) {
        ipInt[i] = std::stoi(ipPart);
        i++;
    }

    if (i != 4) {
        throw std::invalid_argument("Invalid IP address format: " + ip);
    }

    return ipInt;
}

int main(int ac, char **argv) {
    std::string serverHost = argv[1];
    uint16_t serverPort = std::stoi(argv[2]);

    try {
        std::cout << "Connecting to server: " << serverHost << ": UDP=> " << serverPort << " TCP=> " << (serverPort + 1) << std::endl;
        ClientUDP<Communication::TypeDetail> client(serverHost, serverPort);
        ClientTCP<Communication::TypeDetail> client2(serverHost, (serverPort + 1));
        Message<Communication::TypeDetail> msg;
        msg.header.type = {Communication::Type::ConnexionDetail, Communication::main::ConnexionDetailPrecision::ClientConnexion};
        msg.header.size = 0;
        client.Send(msg);
        client2.Send(msg);
        std::cout << "Sent message to server" << std::endl;

        std::string input;
        while (std::getline(std::cin, input)) {
            if (input.empty()) break;

            if (input == "send") {
                Message<Communication::TypeDetail> msg2;
                msg2.header.type = {Communication::Type::EntityInfo, Communication::main::EntityInfoPrecision::InfoAll};
                msg2.header.size = 0;
                client.Send(msg2);
                client2.Send(msg2);
                std::cout << "Sent message: " << input << std::endl;
            }
            input.clear();
        }
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
