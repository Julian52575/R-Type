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
#include "../Config/TinyArrowConfig.hpp"
#include "AState.hpp"
#include "src/State/State.hpp"
#include <SFML/System/Clock.hpp>

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
        GameScenesMenu,
        GameScenesGameOver
    };

    enum GameMenuScene {
        GameMenuSceneNa,
        GameMenuSceneFight,
        GameMenuSceneAct,
        GameMenuSceneItem,
        GameMenuSceneMercy
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
            * @brief Play the boss attack.
            */
            friend State playFunction(GameState& gameState);

            /*
            * @fn playMenu
            * @param GameState A reference to this GameState class.
            * @brief Play the menu in the boss fight
            */
            friend State playMenu(GameState& gameState);

            /*
            * @fn playGameOver
            * @param GameState A reference to this GameState class.
            * @brief Play the game over screen.
            */
            friend State playGameOver(GameState& gameState);

        /*      Player management       */
        private:
            void initScenePlay(void);

        // Graphics
        protected:
            Rengine::Graphics::GraphicManagerSingletone _graphicManagerSingletone;

        public:
            void drawButton(GameState& gameState);
            void setOpacity(GameState& gameState, int opacity);
            void setSpriteOpacity(std::shared_ptr<Rengine::Graphics::ASprite>& sprite, int opacity);

        // Helper
        private:
            void initScenes(void);
            
            // Undyne
            std::shared_ptr<Rengine::Graphics::ASprite> _undyne;
            std::shared_ptr<Rengine::Graphics::ASprite> _undyneRedBar;
            std::shared_ptr<Rengine::Graphics::ASprite> _undyneGreenBar;
            float _undyneMaxHealth = 10000;
            int _undyneHealth = 10000;
            bool _undyneOpacitySet = false;
            bool _undyneOpacityUnset = false;

            std::unique_ptr<Config::TinyArrowConfig> _tinyArrowConfig;
            bool _attackSet = false;

            Config::FloatRect _undyneRect;
            std::shared_ptr<Rengine::Graphics::ASprite> _undyneRectSprite;
            bool _waitingForStart;
            bool _waitingBeforeMenu;
            bool _bossAttacking;
            sf::Time _startWait;
            sf::Time _menuWait;

            // Player
            void updatePlayerHealth(GameState& gameState);
            std::shared_ptr<Rengine::Graphics::ASprite> _player;
            int _playerHealth = 56;

            // menu buttons
            std::shared_ptr<Rengine::Graphics::ASprite> _act_unselected;
            std::shared_ptr<Rengine::Graphics::ASprite> _act_selected;
            std::shared_ptr<Rengine::Graphics::ASprite> _fight_unselected;
            std::shared_ptr<Rengine::Graphics::ASprite> _fight_selected;
            std::shared_ptr<Rengine::Graphics::ASprite> _item_unselected;
            std::shared_ptr<Rengine::Graphics::ASprite> _item_selected;
            std::shared_ptr<Rengine::Graphics::ASprite> _mercy_unselected;
            std::shared_ptr<Rengine::Graphics::ASprite> _mercy_selected;
            std::shared_ptr<Rengine::Graphics::ASprite> _heart;

            // heart in menu
            Rengine::Graphics::vector2D<float> _heartPos;
            short int _heartCol = 0;

            short int _menuRow = 0;
            short int _menuCol = 0;

            // Game scenes
            GameMenuScene _currentMenuScene = GameMenuSceneNa;

            // Inside menu text
            std::shared_ptr<Rengine::Graphics::AText> _insideMenuText;
            std::shared_ptr<Rengine::Graphics::AText> _fightText;
            std::shared_ptr<Rengine::Graphics::AText> _actText;
            std::shared_ptr<Rengine::Graphics::AText> _itemText;
            std::shared_ptr<Rengine::Graphics::AText> _mercyText;

            std::shared_ptr<Rengine::Graphics::AText> _pieText;
            bool _itemSelected = false;

            std::shared_ptr<Rengine::Graphics::AText> _checkText;
            bool _checkSelected = false;

            std::shared_ptr<Rengine::Graphics::ASprite> _playerAttack;
            Rengine::Graphics::vector2D<float> _playerAttackPos;
            std::shared_ptr<Rengine::Graphics::ASprite> _playerBackground;
            bool _playerAttackSelected = false;
            bool _startPlayerAttack = false;
            bool _displayUndyneHeatlh = false;
            bool _swapToUndyne = false;
            sf::Time _waitBeforeAttack;
            sf::Time _playerAttackTime;
            sf::Clock _playerAttackClock;

            // info bar
            std::shared_ptr<Rengine::Graphics::ASprite> _rect;
            std::shared_ptr<Rengine::Graphics::AText> _playerName;
            std::shared_ptr<Rengine::Graphics::AText> _playerLvl;
            std::shared_ptr<Rengine::Graphics::AText> _hpText;
            std::shared_ptr<Rengine::Graphics::AText> _healthText;

            std::shared_ptr<Rengine::Graphics::ASprite> _redhealthbar;
            std::shared_ptr<Rengine::Graphics::ASprite> _yellowhealthlbar;

            // Game clock
            sf::Clock _gameClock;
            sf::Time _buttonTime;


            // Game over
            std::shared_ptr<Rengine::Graphics::ASprite> _gameOver;
            int _opacity = 0;
            sf::Time _opacityTime;

            std::shared_ptr<Rengine::Graphics::ASound> _backgroundMusic;
    };
}  // namespace Undertale
#endif  // SRC_STATE_GAMESTATE_HPP_
