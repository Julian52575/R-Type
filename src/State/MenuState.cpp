//
#include "MenuState.hpp"
#include "src/State/State.hpp"
#include <exception>
#include <functional>
#include <rengine/src/Graphics/AText.hpp>
#include <rengine/src/Graphics/GraphicManager.hpp>
#include <rengine/src/Graphics/SoundSpecs.hpp>
#include <rengine/src/Graphics/TextSpecs.hpp>
#include <rengine/src/Graphics/UserInputManager.hpp>
#include <string>

namespace Undertale {

    MenuState::MenuState(Rengine::ECS& ecs) : AState(ecs)
    {
        this->initScenes();
        this->_sceneManager.setScene(MenuScenesButtonDisplay);
        // Background music
        // Rengine::Graphics::SoundSpecs specs;

        // specs.soundPath = "assets/musics/starforx64_training_mode.mp3";
        // specs.loop = true;
        // this->_backgroundMusic = Rengine::Graphics::GraphicManagerSingletone::get().createSound(specs);
        // this->_backgroundMusic->play();
    }

    void MenuState::registerComponents(void)
    {
    }

    State MenuState::run(void)
    {
        State s = this->_sceneManager.callCurrentSceneFunction<State>(*this);

        return s;
    }

    State buttonDisplay(MenuState& thiss)
    {
        thiss.buttonDisplayHandleInput();
        // Display logo
        Rengine::Graphics::GraphicManagerSingletone::get().addToRender(thiss._logo, {120, 50});
        // _cursor
        if (thiss.currentButton == 0) {
            Rengine::Graphics::GraphicManagerSingletone::get().addToRender(thiss._cursor, {260, 260});
        } else {
            Rengine::Graphics::GraphicManagerSingletone::get().addToRender(thiss._cursor, {260, 310});
        }
        // Display button and input
        Rengine::Graphics::GraphicManagerSingletone::get().addToRender(thiss._buttonVector[0], {300, 250});
        Rengine::Graphics::GraphicManagerSingletone::get().addToRender(thiss._buttonVector[1], {300, 300});
        if (thiss.play) {
            return StateGame;
        }
        return StateMenu;
    }

    void MenuState::buttonDisplayHandleInput(void)
    {
        Rengine::Graphics::UserInputManager inputManager = Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager();
        std::optional<std::reference_wrapper<const Rengine::Graphics::UserInput>> charInput = inputManager.receivedInput(Rengine::Graphics::UserInputTypeKeyboardChar);
        std::optional<std::reference_wrapper<const Rengine::Graphics::UserInput>> spInput = inputManager.receivedInput(Rengine::Graphics::UserInputTypeKeyboardSpecial);

        for (auto it : inputManager) {
            if (it.type == Rengine::Graphics::UserInputTypeKeyboardSpecial) {
                if (it.data.keyboardSpecial == Rengine::Graphics::UserInputKeyboardSpecialArrowDOWN) {
                    this->currentButton = 1;
                } else if (it.data.keyboardSpecial == Rengine::Graphics::UserInputKeyboardSpecialArrowUP) {
                    this->currentButton = 0;
                }
            }
            if (it.type == Rengine::Graphics::UserInputTypeKeyboardChar) {
                if (it.data.keyboardChar == 'z') {
                    if (this->currentButton == 0) {
                        if (this->play) {
                            return;
                        }
                        this->play = true;
                        return;
                    } else if (this->currentButton == 1) {
                        Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->close();
                    }
                }
            }
        }
    }

    void MenuState::buttonDisplayInitButtons(void)
    {
        this->_buttonVector.resize(MenuStateButtons::MenuStateButtonsMax);
        Rengine::Graphics::TextSpecs specs;

        // specs
        specs.style = Rengine::Graphics::TextStyle::TextStyleBold;
        specs.color = {255, 255, 255};
        specs.characterSize = 30;
        specs.outlineColor = {0, 0, 0};
        specs.fontPath = "assets/fonts/8bitoperator_jve.ttf";
        specs.message = "Play";
        this->_buttonVector[MenuStateButtonPlay] = {
                Rengine::Graphics::GraphicManagerSingletone::get().createText(specs),
        };

        specs.message = "Quit";
        this->_buttonVector[MenuStateButtonQuit] = {
            Rengine::Graphics::GraphicManagerSingletone::get().createText(specs),
        };
        Rengine::Graphics::GraphicManager& manager = Rengine::Graphics::GraphicManagerSingletone::get();
        Rengine::Graphics::SpriteSpecs spriteSpecs;

        spriteSpecs.type = Rengine::Graphics::SpriteTypeSprite;
        spriteSpecs.texturePath = "assets/images/undertale_logo.png";
        this->_logo = manager.createSprite(spriteSpecs);

        spriteSpecs.texturePath = "assets/images/heart.png";
        spriteSpecs.textureScale = {0.08, 0.08};
        this->_cursor = manager.createSprite(spriteSpecs);
    }

    void MenuState::initScenes(void)
    {
        std::function<State(MenuState&)> buttonDisplayFun(buttonDisplay);

        this->_sceneManager.setSceneFunction<State, MenuState&>(MenuScenesButtonDisplay, buttonDisplayFun);
        this->buttonDisplayInitButtons();
    }
}  // namespace Undertale
