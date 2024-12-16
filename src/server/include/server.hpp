#pragma once
#include <rengine/Rengine.hpp>
#include <rengine/RengineNetwork.hpp>
#include <map>
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <stdint.h>

#pragma once

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

/**
 * @brief Represents the server responsible for managing game entities and handling messages.
 */
class Server {
private:
    registry reg;  ///< The registry for managing entities.
    EntityManager em;  ///< The entity manager for creating and managing entities.
    std::unique_ptr<EntityMaker> maker;  ///< The maker responsible for creating entities.
    ServerConnection<Communication::TypeDetail> server;  ///< The server connection for communication.
    bool isRunning = true;  ///< Indicates whether the server is running.

    /**
     * @brief Handles incoming messages.
     * 
     * This method processes messages received by the server and performs actions based on the message type and precision.
     * 
     * @param msg The message to handle.
     * @param endpoint The endpoint from which the message originated.
     */
    void handleMessage(Message<Communication::TypeDetail> &msg, const asio::ip::udp::endpoint &endpoint);

    std::unordered_map<User, Entity> users;  ///< Map of users and their associated entities.

    System::movement movement;  ///< System for managing entity movement.
    System::Collision collision;  ///< System for handling entity collisions.
    System::Lifetime lifetimeSystem;  ///< System for managing entity lifetimes.
    System::Script script;  ///< System for executing scripts.

public:
    /**
     * @brief Constructs a new server instance.
     * 
     * @param port The port on which the server listens for connections.
     */
    explicit Server(int32_t port);

    /**
     * @brief Destroys an entity.
     * 
     * @param e The entity to destroy.
     */
    void destroy_entity(Entity e);

    /**
     * @brief Runs the server.
     * 
     * This method starts the server and processes incoming messages.
     */
    void run();

    /**
     * @brief Creates a new entity.
     * 
     * @param configurationId The configuration ID for the entity.
     * @return A reference to the newly created entity.
     */
    Entity &MakeEntity(uint16_t configurationId);

    /**
     * @brief Gets the server connection.
     * 
     * @return A reference to the server connection.
     */
    ServerConnection<Communication::TypeDetail> &getServer() { return server; }

    /**
     * @brief Gets the map of users and their associated entities.
     * 
     * @return A reference to the map of users and entities.
     */
    std::unordered_map<User, Entity> &getUsers() { return users; }

    /**
     * @brief Gets the entity maker.
     * 
     * @return A unique pointer to the entity maker.
     */
    std::unique_ptr<EntityMaker> &getMaker() { return maker; }

    /**
     * @brief Gets the active entities.
     * 
     * @return A vector containing the active entities.
     */
    std::vector<Entity> getEntities() { return em.getActiveEntities(); }

    /**
     * @brief Default destructor for the server.
     */
    ~Server() = default;
};

/**
 * @brief Finds the path for an entity based on its ID.
 * 
 * This function retrieves the file path or resource path for an entity's configuration.
 * 
 * @param id The configuration ID of the entity.
 * @return The path as a string.
 */
std::string find_entity_path(uint16_t id);
