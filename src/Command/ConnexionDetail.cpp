/**
 * @file game.cpp
 * @brief Implements the Game class methods for managing entities, communication, and player state.
 */

#include "../include/game.hpp"

/**
 * @brief Handles receiving a playable entity ID from the server.
 * 
 * This function processes a message of type `PlayableEntityInGameId`. It retrieves 
 * the ID of the entity from the message, creates or retrieves the corresponding 
 * entity, and sets it as the player's entity in the game.
 * 
 * @param msg The message containing the playable entity ID.
 * @param game A reference to the game instance.
 */
static void HandleReceivePlyableEntityId(Message<Communication::TypeDetail> &msg, Game &game) {
    uint16_t id;
    
    msg >> id;
    Entity NewEntity = game.GetOrCreateEntity(game.getEntities(), id, game.getCore(), find_entity_path(3), 3);
    
    game.setPlayer(NewEntity);
}

/**
 * @brief Handles a server disconnect message.
 * 
 * This function processes a message of type `ServerDisconnect`. It retrieves 
 * the ID of the entity to be removed, removes the corresponding entity from 
 * the game's entity list, and erases it from the `EntityMaker` in the core engine.
 * 
 * @param msg The message indicating server disconnect.
 * @param game A reference to the game instance.
 */
static void handleServerDisconnect(Message<Communication::TypeDetail> &msg, Game &game) {
    uint16_t id;

    msg >> id;
    try {
        if (game.getEntities().find(id) != game.getEntities().end()) {
            Entity e = game.getEntities().at(id);
            game.getCore().getEntityMaker().EraseEntity(e);
            game.getEntities().erase(id);
        }
    } catch (std::out_of_range &e) {
        std::cerr << "Entity with id " << id << " not found" << std::endl;
    }
}

/**
 * @brief Handles detailed communication messages based on their precision type.
 * 
 * This method processes communication messages with details specified by their 
 * precision. It delegates handling to the appropriate functions based on the 
 * precision type, such as handling playable entity IDs or server disconnections.
 * 
 * @param msg The message containing communication details.
 * @param game A reference to the game instance.
 */
void Game::handleCommunicationDetail(Message<Communication::TypeDetail> &msg, Game &game) {
    switch (msg.header.type.precision) {
        case Communication::main::ConnexionDetailPrecision::PlayableEntityInGameId: {
            HandleReceivePlyableEntityId(msg, game);
            break;
        }

        case Communication::main::ConnexionDetailPrecision::ServerDisconnect: {
            handleServerDisconnect(msg, game);
            break;
        }

        default:
            std::cerr << "[CLIENT] Unknown message precision " << msg.header.type.precision << std::endl;
            break;
    }
}
