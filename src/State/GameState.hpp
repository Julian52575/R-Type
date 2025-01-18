//
#ifndef SRC_STATE_GAMESTATE_HPP_
#define SRC_STATE_GAMESTATE_HPP_
#include <functional>
#include <memory>
#include <optional>
#include <rengine/Rengine.hpp>
#include <rengine/RengineGraphics.hpp>
#include <rengine/src/Entity.hpp>

#include "../Config/ImageConfig.hpp"
#include "AState.hpp"
#include "src/State/State.hpp"


namespace Undertale {

    /**
    * @addtogroup Undertale
    * @namespace Undertale
    * @class GameScenes
    * @brief The different scenes for the game state.
    */
    enum GameScenes {
        GameScenesNA,
        GameScenesPlay,
        GameScenesGameOver
    };

    enum PlayScene {
        PlaySceneMenu,
        PlaySceneBattle,
    };

    enum MenuScene {
        MenuSceneAct,
        MenuSceneFight,
        MenuSceneItem,
        MenuSceneMercy
    };

    /**
    * @addtogroup Undertale
    * @namespace Undertale
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

            void alertPlayer(void);
            void loadCurrentScene(void);
            void initScenePlay(void);

        protected:
            #define RTYPE_NO_PLAYER_ENTITY_ID (Rengine::ECS::size_type) -1
            Rengine::ECS::size_type _playerEntityId = RTYPE_NO_PLAYER_ENTITY_ID;

        // Graphics
        protected:
            Rengine::Graphics::GraphicManagerSingletone _graphicManagerSingletone;

        // Helper
        private:
            Rengine::Graphics::vector2D<float> getSpriteVecFloat(std::shared_ptr<Rengine::Graphics::ASprite> sprite);
            void initScenes(void);
            std::vector<std::shared_ptr<Rengine::Graphics::ASprite>> _backgroundSprites;
            std::vector<Rengine::Entity> _current_enemies;
            
            // Undyne
            std::shared_ptr<Rengine::Graphics::ASprite> _undyne;


            // menu buttons
            std::shared_ptr<Rengine::Graphics::ASprite> _act_unselected;
            std::shared_ptr<Rengine::Graphics::ASprite> _act_selected;
            std::shared_ptr<Rengine::Graphics::ASprite> _fight_unselected;
            std::shared_ptr<Rengine::Graphics::ASprite> _fight_selected;
            std::shared_ptr<Rengine::Graphics::ASprite> _item_unselected;
            std::shared_ptr<Rengine::Graphics::ASprite> _item_selected;
            std::shared_ptr<Rengine::Graphics::ASprite> _mercy_unselected;
            std::shared_ptr<Rengine::Graphics::ASprite> _mercy_selected;
    };
}  // namespace Undertale
#endif  // SRC_STATE_GAMESTATE_HPP_
