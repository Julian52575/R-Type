//
#include "StateManager.hpp"
#include "State.hpp"
#include "GameState.hpp"

namespace RType {


    StateManager::StateManager(Rengine::ECS& ecs) : _ecs(ecs), _menu(ecs), _game(ecs, this->_menu.getAccessibilitySettings()), _lobby(ecs)
    {
        this->_menu.registerComponents();
        this->_lobby.registerComponents();
        this->_game.registerComponents();
    }

    void StateManager::setState(State newState)
    {
        switch (newState) {
            // Going to lobby
            case State::StateLobby:
                // From menu
                if (this->_currentState == State::StateMenu) {
                    // Pass the lobby info
                    this->_lobby.setLobbyInfo(this->_menu.getLobbyInfo());
                }
                break;

            case State::StateGame:
                if (this->_currentState == State::StateLobby) {
                    this->_game.setNetworkInfo(this->_lobby.getNetworkInfo());
                }
                break;
            // Nothing to do for other change
            default:
                break;
        }
        // Clear ECS when leaving game
        if (this->_currentState == State::StateGame && newState != State::StateGame) {
            this->_ecs.clearEntities();
        }
        this->_currentState = newState;
    }

    void StateManager::run(void)
    {
        State request = StateNA;

        switch (this->_currentState) {
            case State::StateMenu:
                request = this->_menu.run();
                break;

            case State::StateLobby:
                request = this->_lobby.run();
                break;

            case State::StateGame:
                request = this->_game.run();
                break;

            case State::StateNA:
                return;
        }  // switch GameState
        this->setState(request);
    }

    SceneManager& StateManager::getGameSceneManager(void) noexcept
    {
        return this->_game.getSceneManager();
    }

    SceneManager& StateManager::getMenuSceneManager(void) noexcept
    {
        return this->_menu.getSceneManager();
    }

}  // namespace RType
