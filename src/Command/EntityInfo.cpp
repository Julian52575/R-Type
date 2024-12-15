/**
 * @file game.cpp
 * @brief Implements the Game class methods for managing entities, communication, and player state.
 */

#include "include/game.hpp"

/**
 * @brief Handles updating information for all entities.
 * 
 * This function processes a message with information about multiple entities.
 * It retrieves the entity ID, configuration ID, and position (x, y) from the 
 * message, creates or retrieves the corresponding entity, and updates its position.
 * 
 * @param msg The message containing entity information.
 * @param game A reference to the game instance.
 */
static void handleEntityInfoAll(Message<Communication::TypeDetail> &msg, Game &game) {
    uint16_t id, configId;
    float x, y;

    msg >> configId >> y >> x >> id;
    try {
        Entity e = game.GetOrCreateEntity(game.getEntities(), id, game.getCore(), find_entity_path(configId), configId);
        game.getCore().getEntityMaker().UpdatePosition(e, x, y);
    } catch (std::out_of_range &e) {
        std::cerr << "Entity with id " << id << " not found" << std::endl;
    }
}

/**
 * @brief Handles the creation of a new entity.
 * 
 * This function processes a message indicating that a new entity has been created.
 * It retrieves the entity ID, configuration ID, and position (x, y) from the message,
 * creates the corresponding entity, and updates its position.
 * 
 * @param msg The message containing the new entity information.
 * @param game A reference to the game instance.
 */
static void handleEntityNewEntity(Message<Communication::TypeDetail> &msg, Game &game) {
    uint16_t id, configId;
    float x, y;

    msg >> configId >> y >> x >> id;
    Entity e = game.GetOrCreateEntity(game.getEntities(), id, game.getCore(), find_entity_path(configId), configId);
    game.getCore().getEntityMaker().UpdatePosition(e, x, y);
}

/**
 * @brief Handles the deletion of an entity.
 * 
 * This function processes a message indicating that an entity should be deleted.
 * It retrieves the entity ID from the message, checks if it is the player's entity,
 * and removes it from the game's entity list. If the entity is the player's, 
 * the game is marked as finished.
 * 
 * @param msg The message containing the entity deletion information.
 * @param game A reference to the game instance.
 */
static void handleEntityDelete(Message<Communication::TypeDetail> &msg, Game &game) {
    uint16_t id;

    msg >> id;
    try {
        Entity e = game.getEntities().at(id);
        if (game.getPlayer() != nullptr && game.getPlayer()->getId() == e.getId()) {
            game.setFinished(true);
            game.setPlayerNull();
        }
        game.getCore().destroyEntity(e);
    } catch (std::out_of_range &e) {
        std::cerr << "Entity with id " << id << " not found" << std::endl;
    }
}

/**
 * @brief Handles entity-related messages based on their precision type.
 * 
 * This method processes communication messages related to entities. It delegates 
 * handling to the appropriate function based on the message's precision type, 
 * such as updating entity information, creating a new entity, or deleting an entity.
 * 
 * @param msg The message containing entity-related information.
 * @param game A reference to the game instance.
 */
void Game::handleEntityInfo(Message<Communication::TypeDetail> &msg, Game &game) {
    switch (msg.header.type.precision) {
        case Communication::main::EntityInfoPrecision::InfoAll: {
            handleEntityInfoAll(msg, game);
            break;
        }

        case Communication::main::EntityInfoPrecision::NewEntity: {
            handleEntityNewEntity(msg, game);
            break;
        }

        case Communication::main::EntityInfoPrecision::DeleteEntity: {
            handleEntityDelete(msg, game);
            break;
        }

        default:
            break;
    }
}
