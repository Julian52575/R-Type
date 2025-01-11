//
#ifndef SRC_STATE_GAMESTATE_HPP_
#define SRC_STATE_GAMESTATE_HPP_
#include <functional>
#include <memory>
#include <optional>
#include <rengine/Rengine.hpp>
#include <rengine/RengineGraphics.hpp>

#include "src/Config/LevelConfig.hpp"
#include "src/Config/LevelConfigResolver.hpp"
#include "AState.hpp"
#include "src/State/State.hpp"

namespace RType {

    /**
    * @addtogroup RType
    * @namespace RType
    * @class GameScenes
    * @brief The different scenes for the game state.
    */
    enum GameScenes {
        GameScenesNA,
        GameScenesLoadLevel,
        GameScenesPlay,
        GameScenesPause,
        GameScenesGameOver
    };

    /**
    * @addtogroup RType
    * @namespace RType
    * @class GameState
    * @brief This class is used for the game part of the program.
    */
    class GameState : public AState {
        public:
            GameState(Rengine::ECS& ecs);
            ~GameState(void) = default;
            /*
            * @fn registerComponents
            * @brief Registers the neccessary component to the ECS.
            */
            void registerComponents(void);
            /*
            * @fn run
            * @return State The requested next state of the game.
            * @brief Run the program in it's current state.
            */
            State run(void);

        public:
            /*
            * @fn loadLevel
            * @param std::string The path to a level json
            * @brief Set the next level to be loaded
            */
            void setNextLevelToLoad(const std::string& level);
            /*
            * @fn loadLevel
            * @param std::string The path to a level json
            * @brief Load a level.
            */
            void loadLevel(const std::string& jsonPath);
            /*
            * @fn loadLevel
            * @param RType::Config::LevelConfig A reference to a level config
            * @brief Load a level.
            */
            void loadLevel(const RType::Config::LevelConfig& levelConfig);

        public:
            /*
            * @fn loadLevelFunction
            * @param GameState A reference to this GameState class.
            * @brief Load a level and change the scene to play
            */
            friend void loadLevelFunction(GameState& gameState);
            /*
            * @fn playFunction
            * @param GameState A reference to this GameState class.
            * @brief Play the game from the currently loaded level.
            */
            friend State playFunction(GameState& gameState);

        /*      Player management       */
        private:
            /*
            * @fn createPlayer
            * @param std::string A path to an entity config
            * @brief Creates the player entity from the config.
            */
            void createPlayer(const std::string& jsonPath);
            /*
            * @fn deletePlayer
            * @brief Deletes the player entity.
            */
            void deletePlayer(void);
            /*
            * @fn sendInputToPlayerEntity
            * @brief Sends the input from the window to the player entity.
            */
            void sendInputToPlayerEntity(void);

            void createBackground(const std::string& jsonPath);

        protected:
        #define RTYPE_NO_PLAYER_ENTITY_ID (Rengine::ECS::size_type) -1
            Rengine::ECS::size_type _playerEntityId = RTYPE_NO_PLAYER_ENTITY_ID;

        // Graphics
        protected:
            Rengine::Graphics::GraphicManagerSingletone _graphicManagerSingletone;

        // Helper
        private:
            std::string _levelToLoad = "";
            void initScenes(void);
    };
}  // namespace RType
#endif  // SRC_STATE_GAMESTATE_HPP_
