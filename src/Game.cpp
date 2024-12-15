/**
 * @file game.cpp
 * @brief Implements the Game class methods for managing entities, communication, and player state.
 */

#include "include/game.hpp"

/**
 * @brief Retrieves or creates an entity in the game.
 * 
 * If the entity with the given ID does not exist in the entities map, it creates a new entity 
 * using the provided `Rengine` instance, adds it to the map, and sets its configuration ID.
 * 
 * @param entities The map of existing entities.
 * @param id The ID of the entity.
 * @param core The Rengine instance for creating entities.
 * @param path The resource path for the entity.
 * @param configId The configuration ID to assign to the entity.
 * @return A reference to the entity in the map.
 */
Entity &Game::GetOrCreateEntity(std::unordered_map<uint16_t, Entity> &entities, uint16_t id, Rengine::Rengine &core, std::string path, uint16_t configId) {
    if (entities.find(id) == entities.end()) {
        Entity e = core.makeEntity(path);
        entities.emplace(id, e);
        e.setConfigId(configId);
    }
    return entities.at(id);
}

/**
 * @brief Constructs a new Game instance and initializes the client connection.
 * 
 * @param ip The server's IP address.
 * @param port The server's port number.
 */
Game::Game(std::string ip, uint16_t port) : client(ip, port) {
    player = nullptr;
}

/**
 * @brief Sets the finished state of the game.
 * 
 * @param finished True if the game is finished, false otherwise.
 */
void Game::setFinished(bool finished) {
    this->finished = finished;
}

/**
 * @brief Checks if the game is finished.
 * 
 * @return True if the game is finished, false otherwise.
 */
bool Game::isFinished() const {
    return finished;
}

/**
 * @brief Handles incoming messages and dispatches them based on their type.
 * 
 * This method processes messages received from the network and delegates handling 
 * to appropriate methods based on the communication type.
 * 
 * @param msg The received message.
 */
void Game::handleMessage(Message<Communication::TypeDetail> &msg) {
    switch (msg.header.type.type) {
        case Communication::ConnexionDetail:
            this->handleCommunicationDetail(msg, *this);
            break;
        case Communication::JsonDetail:
            this->handleJsonDetail(msg, *this);
            break;
        case Communication::EntityAction:
            this->handleEntityAction(msg, *this);
            break;
        case Communication::EntityInfo:
            this->handleEntityInfo(msg, *this);
            break;
        default:
            std::cout << "Unknown message type " << msg.header.type.type << std::endl;
            break;
    }
}

/**
 * @brief Retrieves the map of entities in the game.
 * 
 * @return A reference to the map of entities.
 */
std::unordered_map<uint16_t, Entity> &Game::getEntities() {
    return entities;
}

/**
 * @brief Retrieves the core engine instance.
 * 
 * @return A reference to the `Rengine` instance.
 */
Rengine::Rengine &Game::getCore() {
    return core;
}

/**
 * @brief Retrieves the current player entity.
 * 
 * @return A shared pointer to the player entity.
 */
std::shared_ptr<Entity> Game::getPlayer() {
    return player;
}

/**
 * @brief Retrieves the client connection instance.
 * 
 * @return A reference to the client connection.
 */
ClientConnection<Communication::TypeDetail> &Game::getClient() {
    return client;
}

/**
 * @brief Sets the player entity and marks it as controllable.
 * 
 * This method assigns the given entity as the player entity and updates the core 
 * engine to mark the entity as controllable.
 * 
 * @param pl The entity to set as the player.
 */
void Game::setPlayer(Entity &pl) {
    this->player = std::make_shared<Entity>(pl);
    core.getEntityMaker().controllable.emplace_at(pl);
    std::cout << "Player set: " << pl << std::endl;
}

/**
 * @brief Sets the player entity to null.
 */
void Game::setPlayerNull() {
    this->player = nullptr;
}

/**
 * @brief Destructor for the Game class.
 * 
 * Cleans up any resources used by the game instance.
 */
Game::~Game() {
}
