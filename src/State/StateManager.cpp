//
#include "StateManager.hpp"
#include "State.hpp"
#include "GameState.hpp"
#include <rengine/src/Graphics/GraphicManager.hpp>

namespace RType {


    StateManager::StateManager(Rengine::ECS& ecs)
        : _ecs(ecs),
        _lobby(ecs, this->_lobbyInfo,this->_networkInfo),
        _menu(ecs, this->_lobbyInfo, this->_networkInfo, this->_accessibilitySettings),
        _game(ecs, this->_accessibilitySettings, this->_networkInfo)
    {
        this->_menu.registerComponents();
        this->_lobby.registerComponents();
        this->_game.registerComponents();
    }

    void StateManager::setState(State newState)
    {
        // Special cases
        switch (newState) {
            // Exit game on NA
            case State::StateNA:
                Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->close();
                return;
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
