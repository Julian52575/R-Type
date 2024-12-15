/**
 * @file game.cpp
 * @brief Implements the Game class methods for managing entities, communication, and player state.
 */

#include "../include/game.hpp"

/**
 * @brief Handles a message containing entity action details.
 * 
 * This method processes a message of type `EntityAction`. The implementation 
 * should define how the game responds to specific actions performed by entities, 
 * such as movement, shooting, or other in-game actions.
 * 
 * @param msg The message containing entity action details.
 * @param game A reference to the game instance.
 */
void Game::handleEntityAction(Message<Communication::TypeDetail> &msg, Game &game) {
}
