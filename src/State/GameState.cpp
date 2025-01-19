#include <climits>
#include <exception>
#include <filesystem>
#include <functional>
#include <optional>
#include <rengine/Rengine.hpp>
#include <rengine/RengineGraphics.hpp>
#include <rengine/src/ECS.hpp>
#include <rengine/src/Entity.hpp>
#include <rengine/src/Graphics/AWindow.hpp>
#include <rengine/src/Graphics/GraphicManager.hpp>
#include <rengine/src/Graphics/UserInputManager.hpp>

#include "State.hpp"
#include "GameState.hpp"
#include "AState.hpp"

namespace Undertale {

    GameState::GameState(Rengine::ECS& ecs)
        : AState(ecs)
    {
        this->initScenes();
        this->_sceneManager.setScene(GameScenes::GameScenesMenu);
    }

    void GameState::registerComponents(void)
    {
    }

    State GameState::run(void)
    {
        State s = this->_sceneManager.callCurrentSceneFunction<State, GameState&>(*this);

        return s;
    }

    void destroyEntity(Rengine::ECS& ecs, Rengine::Entity& entity)
    {
    }

    State playFunction(GameState& gameState)
    {
        if (!gameState._undyneOpacitySet) {
            Rengine::Graphics::SpriteSpecs undyneSpecs = gameState._undyne->getSpriteSpecs();
            undyneSpecs.opacity = 255;
            gameState._undyne->updateSpriteSpecs(undyneSpecs);
            gameState._undyneOpacitySet = true;
            gameState._undyneOpacityUnset = false;
        }
        auto manager = Rengine::Graphics::GraphicManagerSingletone::get();
        manager.addToRender(gameState._undyne, {225, 5});

        manager.addToRender(gameState._act_unselected, {50, 430});
        manager.addToRender(gameState._fight_unselected, {200, 430});
        manager.addToRender(gameState._item_unselected, {350, 430});
        manager.addToRender(gameState._mercy_unselected, {500, 430});

        manager.addToRender(gameState._undyneRectSprite, {gameState._undyneRect._left, gameState._undyneRect._top});

        manager.addToRender(gameState._playerName, {30, 385});
        manager.addToRender(gameState._playerLvl, {200, 385});
        manager.addToRender(gameState._hpText, {380, 390});
        manager.addToRender(gameState._redhealthbar, {420, 395});
        manager.addToRender(gameState._yellowhealthlbar, {420, 395});
        manager.addToRender(gameState._healthText, {480, 385});

        if (gameState._waitingForStart) {
            if (!gameState._attackSet) {
                gameState._tinyArrowConfig = std::make_unique<Config::TinyArrowConfig>("assets/attacks/tiny_arrows1.json");
                gameState._attackSet = true;
            }
            gameState._startWait += gameState._gameClock.getElapsedTime();
            if (gameState._startWait.asSeconds() > 0.1) {
                gameState._waitingForStart = false;
                gameState._bossAttacking = true;
                gameState._startWait = sf::Time::Zero;
            }
        } else {
            if (gameState._bossAttacking) {
                gameState._tinyArrowConfig->draw();
                gameState._tinyArrowConfig->update();
                if (gameState._tinyArrowConfig->checkCollision(gameState._undyneRect)) {
                    gameState._playerHealth -= 15;
                    gameState.updatePlayerHealth(gameState);
                }
                if (gameState._tinyArrowConfig->attackOver()) {
                    gameState._bossAttacking = false;
                    gameState._waitingBeforeMenu = true;
                }
            }
        }
        if (gameState._waitingBeforeMenu) {
            gameState._tinyArrowConfig->draw();
            gameState._menuWait += gameState._gameClock.getElapsedTime();
            if (gameState._menuWait.asSeconds() > 1.5) {
                gameState._sceneManager.setScene(GameScenes::GameScenesMenu);
                gameState._attackSet = false;
                gameState._tinyArrowConfig.release();
                gameState._menuWait = sf::Time::Zero;
                gameState._waitingBeforeMenu = false;
            }
        }
        gameState._gameClock.restart();

        return StateGame;
    }

    State playMenu(GameState& gameState)
    {
        if (!gameState._undyneOpacityUnset) {
            Rengine::Graphics::SpriteSpecs undyneSpecs = gameState._undyne->getSpriteSpecs();
            undyneSpecs.opacity = 30;
            gameState._undyne->updateSpriteSpecs(undyneSpecs);
            gameState._undyneOpacityUnset = true;
            gameState._undyneOpacitySet = false;
        }
        auto manager = Rengine::Graphics::GraphicManagerSingletone::get();
        manager.addToRender(gameState._undyne, {225, 5});

        manager.addToRender(gameState._rect, {20, 230});

        manager.addToRender(gameState._playerName, {30, 385});
        manager.addToRender(gameState._playerLvl, {200, 385});
        manager.addToRender(gameState._hpText, {380, 390});
        manager.addToRender(gameState._redhealthbar, {420, 395});
        manager.addToRender(gameState._yellowhealthlbar, {420, 395});
        manager.addToRender(gameState._healthText, {480, 385});

        Rengine::Graphics::UserInputManager inputManager = Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager();
        inputManager.receivedInput(Rengine::Graphics::UserInputTypeKeyboardChar);
        inputManager.receivedInput(Rengine::Graphics::UserInputTypeKeyboardSpecial);

        gameState._buttonTime += gameState._gameClock.getElapsedTime();
        switch (gameState._currentMenuScene)
        {
        case GameMenuSceneNa:

            if (gameState._buttonTime.asSeconds() > 0.15) {
                for (auto it : inputManager) {
                    if (it.type == Rengine::Graphics::UserInputTypeKeyboardSpecial) {
                        if (it.data.keyboardSpecial == Rengine::Graphics::UserInputKeyboardSpecialArrowLEFT) {
                            gameState._heartCol -= 1;
                            if (gameState._heartCol < 0) {
                                gameState._heartCol = 0;
                            }
                            gameState._buttonTime = sf::Time::Zero;
                        } else if (it.data.keyboardSpecial == Rengine::Graphics::UserInputKeyboardSpecialArrowRIGHT) {
                            gameState._heartCol += 1;
                            if (gameState._heartCol > 3) {
                                gameState._heartCol = 3;
                            }
                            gameState._buttonTime = sf::Time::Zero;
                        }
                    }
                    if (it.type == Rengine::Graphics::UserInputTypeKeyboardChar) {
                        if (it.data.keyboardChar == 'z') {
                            gameState._currentMenuScene = static_cast<GameMenuScene>(gameState._heartCol + 1);
                            gameState._buttonTime = sf::Time::Zero;
                        }
                    }
                }
            }
            gameState.drawButton(gameState);
            manager.addToRender(gameState._insideMenuText, {80, 250});
            break;

        case GameMenuSceneFight:
            if (gameState._buttonTime.asSeconds() > 0.15) {
                for (auto it : inputManager) {
                    if (gameState._displayUndyneHeatlh) {
                        break;;
                    }
                    if (it.type == Rengine::Graphics::UserInputTypeKeyboardChar) {
                        if (it.data.keyboardChar == 'x') {
                            gameState._currentMenuScene = GameMenuSceneNa;
                            gameState._buttonTime = sf::Time::Zero;
                        }
                        if (it.data.keyboardChar == 'z') {
                            if (!gameState._playerAttackSelected) {
                                gameState._playerAttackSelected = true;
                                gameState._buttonTime = sf::Time::Zero;
                                gameState._waitBeforeAttack = sf::Time::Zero;
                                gameState._playerAttackTime = sf::Time::Zero;
                                gameState._gameClock.restart();
                            } else {
                                gameState._playerAttackSelected = false;
                                gameState._startPlayerAttack = false;
                                gameState._displayUndyneHeatlh = true;
                                gameState._undyneHealth -= 1500;
                                float healthPercent = (gameState._undyneHealth / 15000.) * 120;
                                healthPercent = healthPercent < 0 ? 0 : healthPercent;
                                Rengine::Graphics::SpriteSpecs specs = gameState._undyneGreenBar->getSpriteSpecs();
                                specs.shapeData.specifics.rectangleSize = {healthPercent, 25};
                                gameState._undyneGreenBar->updateSpriteSpecs(specs);
                            }
                        }
                    }
                }
            }

            // handle attack
            if (gameState._playerAttackSelected) {
                if (gameState._startPlayerAttack) {
                    sf::Time elaspedAttackTime = gameState._gameClock.getElapsedTime();
                    gameState._playerAttackTime += elaspedAttackTime;
                    if (gameState._playerAttackTime.asMilliseconds() > 10) {
                        gameState._playerAttackPos.x += gameState._playerAttackTime.asMilliseconds() / 3.5;
                        gameState._playerAttackTime = sf::Time::Zero;
                    }
                } else {
                    gameState._waitBeforeAttack += gameState._gameClock.getElapsedTime();
                    if (gameState._waitBeforeAttack.asSeconds() > 0.3) {
                        gameState._startPlayerAttack = true;
                        gameState._waitBeforeAttack = sf::Time::Zero;
                    }
                }
                gameState._gameClock.restart();
            }

            if (gameState._displayUndyneHeatlh) {
                sf::Time elaspedAttackTime = gameState._gameClock.getElapsedTime();
                gameState._playerAttackTime += elaspedAttackTime;
                if (gameState._playerAttackTime.asSeconds() > 2) {
                    gameState._swapToUndyne = true;
                    gameState._displayUndyneHeatlh = false;
                    gameState._playerAttackTime = sf::Time::Zero;
                    gameState._playerAttackPos = {25, 232};
                }
                manager.addToRender(gameState._undyneRedBar, {400, 75});
                manager.addToRender(gameState._undyneGreenBar, {400, 75});
            }


            // draw attack and menu
            gameState.drawButton(gameState);
            if (!gameState._playerAttackSelected && !gameState._displayUndyneHeatlh && !gameState._swapToUndyne) {
                manager.addToRender(gameState._fightText, {80, 250});
                manager.addToRender(gameState._heart, {45, 260});
                manager.addToRender(gameState._undyneRedBar, {400, 260});
                manager.addToRender(gameState._undyneGreenBar, {400, 260});
            } else {
                manager.addToRender(gameState._playerBackground, {21, 228});
                manager.addToRender(gameState._playerAttack, gameState._playerAttackPos);
            }
            if (gameState._swapToUndyne) {
                gameState._currentMenuScene = GameMenuSceneNa;
                gameState._sceneManager.setScene(GameScenes::GameScenesPlay);
                gameState._swapToUndyne = false;
                gameState._waitingForStart = true;
                gameState._waitingBeforeMenu = false;
                gameState._undyneOpacityUnset = false;
            }
            break;

        case GameMenuSceneAct:
            if (gameState._buttonTime.asSeconds() > 0.15) {
                for (auto it : inputManager) {
                    if (it.type == Rengine::Graphics::UserInputTypeKeyboardChar) {
                        if (it.data.keyboardChar == 'x') {
                            if (!gameState._checkSelected) {
                                gameState._currentMenuScene = GameMenuSceneNa;
                                gameState._buttonTime = sf::Time::Zero;
                            }
                        }
                        if (it.data.keyboardChar == 'z') {
                            if (gameState._checkSelected) {
                                gameState._currentMenuScene = GameMenuSceneNa;
                                gameState._buttonTime = sf::Time::Zero;
                                gameState._sceneManager.setScene(GameScenes::GameScenesPlay);
                            } else {
                                gameState._checkSelected = true;
                                gameState._buttonTime = sf::Time::Zero;
                            }
                        }
                    }
                }
            }
            gameState.drawButton(gameState);
            if (!gameState._checkSelected) {
            manager.addToRender(gameState._actText, {80, 250});
            manager.addToRender(gameState._heart, {45, 260});
            } else {
                manager.addToRender(gameState._checkText, {80, 250});
            }
            break;

        case GameMenuSceneItem:
            if (gameState._buttonTime.asSeconds() > 0.15) {
                for (auto it : inputManager) {
                    if (it.type == Rengine::Graphics::UserInputTypeKeyboardChar) {
                        if (it.data.keyboardChar == 'x') {
                            if (!gameState._itemSelected) {
                                gameState._currentMenuScene = GameMenuSceneNa;
                                gameState._buttonTime = sf::Time::Zero;
                            }
                        }
                        if (it.data.keyboardChar == 'z') {
                            if (gameState._itemSelected) {
                                gameState._playerHealth = 56;
                                gameState.updatePlayerHealth(gameState);
                                gameState._currentMenuScene = GameMenuSceneNa;
                                gameState._buttonTime = sf::Time::Zero;
                                gameState._sceneManager.setScene(GameScenes::GameScenesPlay);
                            } else {
                                gameState._itemSelected = true;
                                gameState._buttonTime = sf::Time::Zero;
                            }
                        }
                    }
                }
            }
            gameState.drawButton(gameState);
            if (!gameState._itemSelected) {
                manager.addToRender(gameState._itemText, {80, 250});
                manager.addToRender(gameState._heart, {45, 260});
            } else {
                manager.addToRender(gameState._pieText, {80, 250});
            }
            break;

        case GameMenuSceneMercy:
            if (gameState._buttonTime.asSeconds() > 0.15) {
                for (auto it : inputManager) {
                    if (it.type == Rengine::Graphics::UserInputTypeKeyboardChar) {
                        if (it.data.keyboardChar == 'x') {
                            gameState._currentMenuScene = GameMenuSceneNa;
                            gameState._buttonTime = sf::Time::Zero;
                        }
                        if (it.data.keyboardChar == 'z') {
                            gameState._currentMenuScene = GameMenuSceneNa;
                            gameState._buttonTime = sf::Time::Zero;
                            gameState._sceneManager.setScene(GameScenes::GameScenesPlay);
                        }
                    }
                }
            }
            gameState.drawButton(gameState);
            manager.addToRender(gameState._mercyText, {80, 250});
            manager.addToRender(gameState._heart, {45, 260});
            break;

        default:
            gameState.drawButton(gameState);
            break;
        }

        gameState._gameClock.restart();
        return StateGame;
    }

    void GameState::drawButton(GameState& gameState)
    {
        auto manager = Rengine::Graphics::GraphicManagerSingletone::get();
        int id = gameState._heartCol;
        if (gameState._currentMenuScene != GameMenuSceneNa) {
            manager.addToRender(gameState._fight_unselected, {50, 430});
            manager.addToRender(gameState._act_unselected, {200, 430});
            manager.addToRender(gameState._item_unselected, {350, 430});
            manager.addToRender(gameState._mercy_unselected, {500, 430});
            return;
        }
        if (id == 0 && gameState._currentMenuScene == GameMenuSceneNa) {
            manager.addToRender(gameState._fight_selected, {50, 430});
            manager.addToRender(gameState._heart, {56, 440});
        } else {
            manager.addToRender(gameState._fight_unselected, {50, 430});
        }
        if (id == 1 && gameState._currentMenuScene == GameMenuSceneNa) {
            manager.addToRender(gameState._act_selected, {200, 430});
            manager.addToRender(gameState._heart, {206, 440});
        } else {
            manager.addToRender(gameState._act_unselected, {200, 430});
        }
        if (id == 2 && gameState._currentMenuScene == GameMenuSceneNa) {
            manager.addToRender(gameState._item_selected, {350, 430});
            manager.addToRender(gameState._heart, {356, 440});
        } else {
            manager.addToRender(gameState._item_unselected, {350, 430});
        }
        if (id == 3 && gameState._currentMenuScene == GameMenuSceneNa) {
            manager.addToRender(gameState._mercy_selected, {500, 430});
            manager.addToRender(gameState._heart, {506, 440});
        } else {
            manager.addToRender(gameState._mercy_unselected, {500, 430});
        }


    }

    State playGameOver(GameState& gameState)
    {
        // make heart break animation and game over screen

        return StateGame;
    }

    void GameState::initScenes(void)
    {
        std::function<State(GameState&)> funScene(playFunction);

        this->_sceneManager.setSceneFunction<State, GameState&>(GameScenes::GameScenesPlay, funScene);

        std::function<State(GameState&)> funMenu(playMenu);
        this->_sceneManager.setSceneFunction<State, GameState&>(GameScenes::GameScenesMenu, funMenu);

        std::function<State(GameState&)> funGameOver(playGameOver);
        this->_sceneManager.setSceneFunction<State, GameState&>(GameScenes::GameScenesGameOver, funGameOver);
        initScenePlay();
    }

    void GameState::initScenePlay(void)
    {
        Rengine::Graphics::GraphicManager& manager = Rengine::Graphics::GraphicManagerSingletone::get();

        _undyne = manager.createSprite(Config::ImageConfig("assets/images/undyne.json").getSpecs());

        _act_unselected = manager.createSprite(Config::ImageConfig("assets/images/button_act.json", "assets/images/button_act_unused.png").getSpecs());
        _act_selected = manager.createSprite(Config::ImageConfig("assets/images/button_act.json", "assets/images/button_act_used.png").getSpecs());

        _fight_unselected = manager.createSprite(Config::ImageConfig("assets/images/button_fight.json", "assets/images/button_fight_unused.png").getSpecs());
        _fight_selected = manager.createSprite(Config::ImageConfig("assets/images/button_fight.json", "assets/images/button_fight_used.png").getSpecs());

        _item_unselected = manager.createSprite(Config::ImageConfig("assets/images/button_item.json", "assets/images/button_item_unused.png").getSpecs());
        _item_selected = manager.createSprite(Config::ImageConfig("assets/images/button_item.json", "assets/images/button_item_used.png").getSpecs());

        _mercy_unselected = manager.createSprite(Config::ImageConfig("assets/images/button_mercy.json", "assets/images/button_mercy_unused.png").getSpecs());
        _mercy_selected = manager.createSprite(Config::ImageConfig("assets/images/button_mercy.json", "assets/images/button_mercy_used.png").getSpecs());

        _heart = manager.createSprite(Config::ImageConfig("assets/images/heart.json").getSpecs());


        Rengine::Graphics::SpriteSpecs rectSpecs;
        rectSpecs.type = Rengine::Graphics::SpriteTypeRectangle;
        rectSpecs.color = {0, 0, 0};
        rectSpecs.shapeData.outlineColor = {255, 255, 255};
        rectSpecs.shapeData.outlineThickness = 2;
        rectSpecs.shapeData.specifics.rectangleSize = {600, 150};
        _rect = manager.createSprite(rectSpecs);

        Rengine::Graphics::TextSpecs textSpecs;
        textSpecs.outlineColor = {0, 0, 0};
        textSpecs.fontPath = "assets/fonts/8bitoperator_jve.ttf";
        textSpecs.letterSpacing = 2;
        textSpecs.color = {255, 255, 255};
        textSpecs.characterSize = 30;

        textSpecs.message = "PLAYER6723";
        _playerName = manager.createText(textSpecs);

        textSpecs.message = "LV  10";
        _playerLvl = manager.createText(textSpecs);

        textSpecs.message = "56  /  56";
        _healthText = manager.createText(textSpecs);

        textSpecs.characterSize = 25;
        textSpecs.message = "HP";
        _hpText = manager.createText(textSpecs);

        Rengine::Graphics::SpriteSpecs healtRectSpecs;

        healtRectSpecs.type = Rengine::Graphics::SpriteTypeRectangle;
        healtRectSpecs.color = {255, 0, 0};
        healtRectSpecs.shapeData.outlineThickness = 0;
        healtRectSpecs.shapeData.specifics.rectangleSize = {50, 20};
        _redhealthbar = manager.createSprite(healtRectSpecs);

        healtRectSpecs.color = {255, 255, 0};
        healtRectSpecs.shapeData.specifics.rectangleSize = {50, 20};
        _yellowhealthlbar = manager.createSprite(healtRectSpecs);
        _currentMenuScene = GameMenuSceneNa;

        Rengine::Graphics::TextSpecs insideMenuTextSpecs;
        insideMenuTextSpecs.outlineColor = {0, 0, 0};
        insideMenuTextSpecs.fontPath = "assets/fonts/8bitoperator_jve.ttf";
        insideMenuTextSpecs.letterSpacing = 2;
        insideMenuTextSpecs.color = {255, 255, 255};
        insideMenuTextSpecs.characterSize = 30;

        insideMenuTextSpecs.message = "* The Heroine appears";
        _insideMenuText = manager.createText(insideMenuTextSpecs);

        insideMenuTextSpecs.message = "* Undyne the Undying";
        _fightText = manager.createText(insideMenuTextSpecs);

        insideMenuTextSpecs.message = "* Check";
        _actText = manager.createText(insideMenuTextSpecs);

        insideMenuTextSpecs.message = "* Pie";
        _itemText = manager.createText(insideMenuTextSpecs);

        insideMenuTextSpecs.message = "* Spare";
        _mercyText = manager.createText(insideMenuTextSpecs);

        insideMenuTextSpecs.message = "* You ate the pie.\n* HP maxed out.";
        _pieText = manager.createText(insideMenuTextSpecs);

        insideMenuTextSpecs.message = "* Heroine reformed by her own\n   DETERMINATION to save earth.";
        insideMenuTextSpecs.lineSpacing = 1.2;
        _checkText = manager.createText(insideMenuTextSpecs);

        Rengine::Graphics::SpriteSpecs undyneBarSpecs;
        undyneBarSpecs.type = Rengine::Graphics::SpriteTypeRectangle;
        undyneBarSpecs.color = {255, 0, 0};
        undyneBarSpecs.shapeData.outlineThickness = 0;
        undyneBarSpecs.shapeData.specifics.rectangleSize = {120, 25};
        _undyneRedBar = manager.createSprite(undyneBarSpecs);

        undyneBarSpecs.color = {0, 255, 0};
        undyneBarSpecs.shapeData.specifics.rectangleSize = {120, 25};
        _undyneGreenBar = manager.createSprite(undyneBarSpecs);

        Rengine::Graphics::SpriteSpecs playerAttackSpecs;
        playerAttackSpecs.type = Rengine::Graphics::SpriteTypeRectangle;
        playerAttackSpecs.color = {255, 255, 255};
        playerAttackSpecs.shapeData.outlineThickness = 0;
        playerAttackSpecs.shapeData.specifics.rectangleSize = {5, 146};
        _playerAttack = manager.createSprite(playerAttackSpecs);
        _playerAttackPos = {25, 232};

        _playerBackground = manager.createSprite(Config::ImageConfig("assets/images/target.json").getSpecs());

        Rengine::Graphics::SpriteSpecs undyneRectSpecs;
        _undyneRect = Config::FloatRect(200, 280, 70, 70);

        undyneRectSpecs.type = Rengine::Graphics::SpriteTypeRectangle;
        undyneRectSpecs.color = {0, 0, 0};
        undyneRectSpecs.shapeData.specifics.rectangleSize = {_undyneRect._width, _undyneRect._height};
        _undyneRectSprite =  manager.createSprite(undyneRectSpecs);
    }

    void GameState::updatePlayerHealth(GameState& gameState)
    {
        float heatlhPercent = (gameState._playerHealth / 56.) * 50;
        heatlhPercent = heatlhPercent < 0 ? 0 : heatlhPercent;
        Rengine::Graphics::SpriteSpecs specs = gameState._yellowhealthlbar->getSpriteSpecs();
        specs.shapeData.specifics.rectangleSize = {heatlhPercent, 20};
        gameState._yellowhealthlbar->updateSpriteSpecs(specs);

        _healthText->setText(std::to_string(_playerHealth) + "  /  56");
    }

    void GameState::setOpacity(GameState& gameState, int opacity)
    {
        std::cout << "set opacity" << std::endl;
        gameState.setSpriteOpacity(gameState._undyne, opacity);
        gameState.setSpriteOpacity(gameState._yellowhealthlbar, opacity);
        gameState.setSpriteOpacity(gameState._redhealthbar, opacity);
    }

    void GameState::setSpriteOpacity(std::shared_ptr<Rengine::Graphics::ASprite>& sprite, int opacity)
    {
        Rengine::Graphics::SpriteSpecs specs = sprite->getSpriteSpecs();
        specs.opacity = opacity;
        sprite->updateSpriteSpecs(specs);
    }
}  // namespace Undertale
