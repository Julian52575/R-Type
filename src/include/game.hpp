/**
 * @file Game.hpp
 * @brief Defines the Communication structure and the Game class for the R-Type networked game.
 */

#pragma once

#include <rengine/Rengine.hpp>
#include <rengine/src/Core/Rengine.hpp>
#include <rengine/RengineNetwork.hpp>
#include <unordered_map>
#include <memory>
#include "./utils.hpp"

/**
 * @struct Communication
 * @brief Contains enumerations and structures for message types and details in the network communication.
 */
struct Communication {
    /**
     * @enum Type
     * @brief Main types of communication messages.
     */
    enum Type : uint16_t {
        NOOP = 0,             /**< No operation */
        ConnexionDetail = 1,  /**< Connection-related details */
        JsonDetail = 2,       /**< JSON-related details */
        EntityAction = 3,     /**< Actions performed by entities */
        EntityInfo = 4,       /**< Information about entities */
    };

    /**
     * @union main
     * @brief Contains precision enums for each communication type.
     */
    union main {
        /**
         * @enum NOOPPrecision
         * @brief Precision details for the NOOP type.
         */
        enum NOOPPrecision : uint16_t {
            NOOP = 0, /**< No operation */
        };

        /**
         * @enum ConnexionDetailPrecision
         * @brief Precision details for connection-related communication.
         */
        enum ConnexionDetailPrecision : uint16_t {
            ClientConnexion = 0,        /**< Client connected */
            ClientDisconnect = 1,      /**< Client disconnected */
            ServerAccept = 2,          /**< Server accepted connection */
            ServerDisconnect = 3,      /**< Server disconnected */
            ClientReady = 4,           /**< Client is ready */
            PlayableEntityInGameId = 5,/**< ID of the playable entity in the game */
            ServerStop = 6,            /**< Server stopped */
            RequestPlaybleEntity = 7,  /**< Request for a playable entity */
        };

        /**
         * @enum JsonDetailPrecision
         * @brief Precision details for JSON-related communication.
         */
        enum JsonDetailPrecision : uint16_t {
            GamePartLevel = 0,  /**< Level details */
            GamePartEntity = 1, /**< Entity details */
            GamePartPlugin = 2, /**< Plugin details */
        };

        /**
         * @enum EntityActionPrecision
         * @brief Precision details for actions performed by entities.
         */
        enum EntityActionPrecision : uint16_t {
            Move = 0,      /**< Move action */
            Shoot1 = 1,    /**< Basic shooting */
            Shoot2 = 2,    /**< Secondary shooting */
            Shoot3 = 3,    /**< Tertiary shooting */
            Dodge = 4,     /**< Dodge action */
            Barrier = 5,   /**< Use barrier */
            Death = 6,     /**< Death event */
            Ultimate = 7   /**< Ultimate ability */
        };

        /**
         * @enum EntityInfoPrecision
         * @brief Precision details for entity-related information.
         */
        enum EntityInfoPrecision : uint16_t {
            InfoAll = 0,                 /**< All information */
            InGameAndConfigurationId = 1,/**< In-game and configuration ID */
            InfoPosition = 2,            /**< Position information */
            InfoHP = 3,                  /**< Current health points */
            InfoMaxHp = 4,               /**< Maximum health points */
            InfoSpeed = 5,               /**< Current speed */
            InfoMaxSpeed = 6,            /**< Maximum speed */
            InfoDirection = 7,           /**< Direction */
            NewEntity = 8,               /**< New entity created */
            DeleteEntity = 9,            /**< Entity deleted */
        };
    };

    /**
     * @struct TypeDetail
     * @brief Represents the type and precision details of a communication message.
     */
    struct TypeDetail {
        Type type;           /**< Type of the communication message */
        uint16_t precision;  /**< Precision value for the type */
    };
};

/**
 * @class Game
 * @brief Represents the main game logic and manages entities, communication, and player state.
 */
class Game {
public:
    /**
     * @brief Constructs a new Game instance.
     * @param ip The server's IP address.
     * @param port The server's port.
     */
    Game(std::string ip, uint16_t port);

    /**
     * @brief Handles incoming messages.
     * @param msg The received message.
     */
    void handleMessage(Message<Communication::TypeDetail> &msg);

    /**
     * @brief Retrieves the map of entities in the game.
     * @return A reference to the map of entities.
     */
    std::unordered_map<uint16_t, Entity> &getEntities();

    /**
     * @brief Retrieves the core engine instance.
     * @return A reference to the Rengine instance.
     */
    Rengine::Rengine &getCore();

    /**
     * @brief Retrieves the client connection instance.
     * @return A reference to the client connection.
     */
    ClientConnection<Communication::TypeDetail> &getClient();

    /**
     * @brief Retrieves the player entity.
     * @return A shared pointer to the player entity.
     */
    std::shared_ptr<Entity> getPlayer();

    /**
     * @brief Sets the player entity.
     * @param player The player entity.
     */
    void setPlayer(Entity &player);

    /**
     * @brief Sets the player to null.
     */
    void setPlayerNull();

    /**
     * @brief Marks the game as finished.
     * @param finished True if the game is finished, false otherwise.
     */
    void setFinished(bool finished);

    /**
     * @brief Checks if the game is finished.
     * @return True if the game is finished, false otherwise.
     */
    bool isFinished() const;

    /**
     * @brief Retrieves or creates an entity in the game.
     * @param entities The map of entities.
     * @param id The ID of the entity.
     * @param core The Rengine instance.
     * @param path The entity's resource path.
     * @param configId The entity's configuration ID.
     * @return A reference to the entity.
     */
    Entity &GetOrCreateEntity(std::unordered_map<uint16_t, Entity> &entities, uint16_t id, Rengine::Rengine &core, std::string path, uint16_t configId);

    /**
     * @brief Destructor for the Game class.
     */
    ~Game();

private:
    ClientConnection<Communication::TypeDetail> client; /**< Client connection for communication */
    std::unordered_map<uint16_t, Entity> entities;      /**< Map of entities in the game */
    Rengine::Rengine core;                              /**< Core game engine */
    std::shared_ptr<Entity> player;                     /**< Player entity */
    bool finished = false;                              /**< Game completion status */

    /**
     * @brief Handles detailed communication messages.
     * @param msg The received message.
     * @param game The game instance.
     */
    void handleCommunicationDetail(Message<Communication::TypeDetail> &msg, Game &game);

    /**
     * @brief Handles JSON-related communication messages.
     * @param msg The received message.
     * @param game The game instance.
     */
    void handleJsonDetail(Message<Communication::TypeDetail> &msg, Game &game);

    /**
     * @brief Handles entity action messages.
     * @param msg The received message.
     * @param game The game instance.
     */
    void handleEntityAction(Message<Communication::TypeDetail> &msg, Game &game);

    /**
     * @brief Handles entity information messages.
     * @param msg The received message.
     * @param game The game instance.
     */
    void handleEntityInfo(Message<Communication::TypeDetail> &msg, Game &game);
};
