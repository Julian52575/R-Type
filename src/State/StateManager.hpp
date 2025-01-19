//
#ifndef SRC_GAME_STATE_STATEMANAGER_HPP_
#define SRC_GAME_STATE_STATEMANAGER_HPP_

#include <cstddef>
#include <memory>
#include <rengine/src/ECS.hpp>

#include "State.hpp"
#include "AState.hpp"
#include "GameState.hpp"
#include "LobbyState.hpp"
#include "Menu/MenuState.hpp"
#include "src/Game/AccessibilitySettings.hpp"

namespace RType {

    /**
    * @addtogroup RType
    * @namespace RType
    * @class GameStateManager
    * @brief Manage the states of the program.
    * See State.hpp
    */
    class StateManager {
        public:
            StateManager(Rengine::ECS& ecs);
            ~StateManager(void) = default;
            /*
            * @fn setState
            * @param GameState The new state of the program.
            * @brief Set the program state.
            */
            void setState(State state);
            /*
            * @fn run
            * @brief Run the program in it's current state.
            */
            void run(void);
            /*
            * @fn getGameSceneManager
            * @return SceneManager The game's scene manager
            * @brief Retrives the game's scene manager.
            */
            SceneManager& getGameSceneManager(void) noexcept;
            /*
            * @fn getMenuSceneManager
            * @return SceneManager The menu's scene manager
            * @brief Retrives the menu's scene manager.
            */
            SceneManager& getMenuSceneManager(void) noexcept;

        // shared structs
        private:
            LobbyInfo _lobbyInfo;
            NetworkInfo _networkInfo;
            AccessibilitySettings _accessibilitySettings;

        private:
            Rengine::ECS& _ecs;
            State _currentState = StateNA;
            MenuState _menu;
            GameState _game;
            LobbyState _lobby;
    };
}  // namespace RType
#endif  // SRC_GAME_STATE_GAMESTATEMANAGER_HPP_
