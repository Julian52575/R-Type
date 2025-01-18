//
#include "StateManager.hpp"
#include "State.hpp"
#include "GameState.hpp"

namespace Undertale {


    StateManager::StateManager(Rengine::ECS& ecs) : _ecs(ecs), _menu(ecs), _game(ecs)
    {
        this->_menu.registerComponents();
        this->_game.registerComponents();
    }

    void StateManager::setState(State newState)
    {
        this->_currentState = newState;
    }

    void StateManager::run(void)
    {
        State request = StateNA;

        switch (this->_currentState) {
            case State::StateMenu:
                request = this->_menu.run();
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

}  // namespace Undertale
