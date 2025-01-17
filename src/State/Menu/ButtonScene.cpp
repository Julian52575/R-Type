//
#include <rengine/src/Graphics/UserInputManager.hpp>
#include <vector>
#include <rengine/RengineGraphics.hpp>

#include "src/State/AScene.hpp"
#include "ButtonScene.hpp"
#include "Scenes.hpp"

namespace RType {

        void ButtonScene::unload(void)
        {
            this->_currentIndex = ButtonsIp;
            this->_backgroundMusic->reset();
        }
        void ButtonScene::reload(void)
        {
            this->_backgroundMusic->play();
        }

        void ButtonScene::display(void)
        {
            float count = 1;
            float yDiff = 100;

            // _cursor
            Rengine::Graphics::GraphicManagerSingletone::get().addToRender(this->_buttonCursor, {50, yDiff * (int) (this->_currentIndex + 1) + 15 });
            // Display button and input
            for (auto it : this->_buttonVector) {
                const std::string& currentField = it.first->getText();

                Rengine::Graphics::GraphicManagerSingletone::get().addToRender(it.first, {50, yDiff * count});
                Rengine::Graphics::GraphicManagerSingletone::get().addToRender(it.second, {550, yDiff * count});
                count += 1;
            }  // it : this->_buttonVector
            if (this->_backgroundMusic->isPlaying() == false) {
                this->_backgroundMusic->play();
            }
        }

        MenuScenes ButtonScene::handleInputs(void)
        {
            Rengine::Graphics::UserInputManager inputManager = Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager();
            std::string currentTextCopy = this->_buttonVector[this->_currentIndex % ButtonsMax].second->getText();
            bool textUpdate = false;
            std::optional<std::reference_wrapper<const Rengine::Graphics::UserInput>> charInput = inputManager.receivedInput(Rengine::Graphics::UserInputTypeKeyboardChar);
            std::optional<std::reference_wrapper<const Rengine::Graphics::UserInput>> spInput = inputManager.receivedInput(Rengine::Graphics::UserInputTypeKeyboardSpecial);

            for (auto it : inputManager) {
                if (it.type == Rengine::Graphics::UserInputTypeKeyboardCharPressed) {
                    currentTextCopy += it.data.keyboardChar;
                    textUpdate = true;
                }
                if (it.type == Rengine::Graphics::UserInputTypeKeyboardSpecialPressed) {
                    switch (it.data.keyboardSpecial) {
                        case Rengine::Graphics::UserInputKeyboardSpecialBACKSPACE:
                            if (currentTextCopy.size() > 0) {
                                currentTextCopy.resize(currentTextCopy.size() - 1);
                                textUpdate = true;
                            }
                            break;

                        case Rengine::Graphics::UserInputKeyboardSpecialTAB:
                            this->_currentIndex = static_cast<Buttons>((this->_currentIndex + 1) % ButtonsMax);
                            break;

                        case Rengine::Graphics::UserInputKeyboardSpecialENTER:
                            return this->exitToLobby();
                            break;

                        case Rengine::Graphics::UserInputKeyboardSpecialESCAPE:
                            return static_cast<MenuScenes>(this->_sceneIndex - 1);

                        default:
                            break;
                    }
                }
            }  // for it
            if (textUpdate == true) {
                this->_buttonVector[this->_currentIndex % ButtonsMax].second->setText(currentTextCopy);
            }
            return MenuScenesButtonDisplay;
        }

        MenuScenes ButtonScene::exitToLobby(void)
        {
            try {
                this->_lobbyInfo.port = std::stoi(this->_buttonVector[ButtonsPort].second->getText());
            } catch (std::exception& e) {
                std::cout << "Exception '" << e.what() << "' when converting port input to int. Staying on menu." << std::endl;
                return MenuScenesButtonDisplay;
            }
            return MenuScenesExitToLobby;
        }

        void ButtonScene::initGraphics(void)
        {
            this->_buttonVector.resize(ButtonsMax);
            // WIP
            Rengine::Graphics::TextSpecs inputSpecs;
            Rengine::Graphics::TextSpecs specs;

            // inputSpecs
            inputSpecs.fontPath = "assets/fonts/arial_narrow_7.ttf";
            inputSpecs.color = {255, 255, 255};
            inputSpecs.message = "";
            inputSpecs.characterSize -= 10;
            inputSpecs.letterSpacing += 2;
            // specs
            specs.style = Rengine::Graphics::TextStyle::TextStyleBold;
            specs.color = {0, 0, 255};
            specs.fontPath = "assets/fonts/arial.ttf";
            specs.message = "Ip address :";
            inputSpecs.message = "0.0.0.0";
            this->_buttonVector[ButtonsIp] = {
                    Rengine::Graphics::GraphicManagerSingletone::get().createText(specs),
                    Rengine::Graphics::GraphicManagerSingletone::get().createText(inputSpecs)
            };

            specs.color = {255, 0, 0};
            specs.message = "Port :";
            inputSpecs.message = "4242";
            this->_buttonVector[ButtonsPort] = {
                Rengine::Graphics::GraphicManagerSingletone::get().createText(specs),
                Rengine::Graphics::GraphicManagerSingletone::get().createText(inputSpecs)
            };
            specs.color = {0, 255, 0};
            specs.message = "Player json :";
            inputSpecs.message = "assets/entities/playerDefault.json";
            this->_buttonVector[ButtonsPlayerJson] = {
                Rengine::Graphics::GraphicManagerSingletone::get().createText(specs),
                Rengine::Graphics::GraphicManagerSingletone::get().createText(inputSpecs)
            };
            Rengine::Graphics::GraphicManager& manager = Rengine::Graphics::GraphicManagerSingletone::get();
            Rengine::Graphics::SpriteSpecs spriteSpecs;

            spriteSpecs.type = Rengine::Graphics::SpriteTypeRectangle;
            spriteSpecs.color = {0, 0, 0};
            spriteSpecs.shapeData.outlineColor = {255, 255, 255};
            spriteSpecs.shapeData.outlineThickness = 5;
            spriteSpecs.shapeData.specifics.rectangleSize = {300, 50};
            this->_buttonCursor = manager.createSprite(spriteSpecs);

            // Background music
            Rengine::Graphics::SoundSpecs musicSpecs;

            musicSpecs.soundPath = "assets/musics/starforx64_training_mode.mp3";
            musicSpecs.loop = true;
                this->_backgroundMusic = Rengine::Graphics::GraphicManagerSingletone::get().createSound(musicSpecs);
        }

}  // namespace RType
