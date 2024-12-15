/**
 * @file game.cpp
 * @brief Implements the Game class methods for managing entities, communication, and player state.
 */

#include "../include/game.hpp"

/**
 * @brief Handles a message containing JSON detail information.
 * 
 * This method processes a message of type `JsonDetail`. The implementation 
 * should define how the JSON-related information is handled in the game.
 * 
 * @param msg The message containing JSON detail information.
 * @param game A reference to the game instance.
 */
void Game::handleJsonDetail(Message<Communication::TypeDetail> &msg, Game &game) {
}
