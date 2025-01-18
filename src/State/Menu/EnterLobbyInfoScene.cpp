//
#include <rengine/src/Graphics/GraphicManager.hpp>
#include <rengine/src/Graphics/SpriteSpecs.hpp>
#include <rengine/src/Graphics/TextSpecs.hpp>
#include <rengine/src/Graphics/UserInputManager.hpp>
#include <vector>
#include <rengine/RengineGraphics.hpp>

#include "src/State/AScene.hpp"
#include "src/State/Menu/Scenes.hpp"
#include "src/State/Menu/EnterLobbyInfoScene.hpp"

namespace RType {

        void EnterLobbyInfoScene::unload(void)
        {
            this->_currentIndex = EnterLobbyButtonsIp;
            this->_backgroundMusic->reset();
            this->_isLoaded = false;
        }
        void EnterLobbyInfoScene::reload(void)
        {
            this->_backgroundMusic->play();
            this->_isLoaded = true;
        }

        void EnterLobbyInfoScene::display(void)
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
            Rengine::Graphics::GraphicManagerSingletone::get().addToRender(this->_newLobbyButton,
                    this->_newLobbyButtonPos
            );
            Rengine::Graphics::GraphicManagerSingletone::get().addToRender(this->_newLobbyButtonText,
                    {this->_newLobbyButtonPos.x + 1,
                    this->_newLobbyButtonPos.y + 10}
            );
            if (this->_backgroundMusic->isPlaying() == false) {
                this->_backgroundMusic->play();
            }
        }

        MenuScenes EnterLobbyInfoScene::handleInputs(void)
        {
            Rengine::Graphics::UserInputManager inputManager = Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager();
            std::string currentTextCopy = this->_buttonVector[this->_currentIndex % EnterLobbyButtonsMax].second->getText();
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
                            this->_currentIndex = static_cast<EnterLobbyButtons>((this->_currentIndex + 1) % EnterLobbyButtonsMax);
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
                if (it.type == Rengine::Graphics::UserInputTypeMouseLeftClick) {
                    if (this->_newLobbyButtonPos.x <= it.data.mousePosition.x
                    && it.data.mousePosition.x <= this->_newLobbyButtonPos.x + this->_newLobbyButton->getSpriteSpecs().shapeData.specifics.rectangleSize.x) {
                        if (this->_newLobbyButtonPos.y <= it.data.mousePosition.y
                        && it.data.mousePosition.y <= this->_newLobbyButtonPos.y + this->_newLobbyButton->getSpriteSpecs().shapeData.specifics.rectangleSize.y) {
                            return MenuScenesCreateLobby;
                        }  // y check
                    }  // x check
                }
            }  // for it
            if (textUpdate == true) {
                this->_buttonVector[this->_currentIndex % EnterLobbyButtonsMax].second->setText(currentTextCopy);
            }
            return MenuScenesEnterLobbyInfo;
        }

        MenuScenes EnterLobbyInfoScene::exitToLobby(void)
        {
            try {
                this->_lobbyInfo.serverIp = this->_buttonVector[EnterLobbyButtonsIp].second->getText();
                this->_lobbyInfo.port = std::stoi(this->_buttonVector[EnterLobbyButtonsPort].second->getText());
                this->_lobbyInfo.playerJson = this->_buttonVector[EnterLobbyButtonsPlayerJson].second->getText();
            } catch (std::exception& e) {
                std::cout << "Exception '" << e.what() << "' when converting port input to int. Staying on menu." << std::endl;
                return MenuScenesEnterLobbyInfo;
            }
            return MenuScenesExitToLobby;
        }

        void EnterLobbyInfoScene::initGraphics(void)
        {
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
            this->_buttonVector[EnterLobbyButtonsIp] = {
                    Rengine::Graphics::GraphicManagerSingletone::get().createText(specs),
                    Rengine::Graphics::GraphicManagerSingletone::get().createText(inputSpecs)
            };

            specs.color = {255, 0, 0};
            specs.message = "Port :";
            inputSpecs.message = "4242";
            this->_buttonVector[EnterLobbyButtonsPort] = {
                Rengine::Graphics::GraphicManagerSingletone::get().createText(specs),
                Rengine::Graphics::GraphicManagerSingletone::get().createText(inputSpecs)
            };
            specs.color = {0, 255, 0};
            specs.message = "Player json :";
            inputSpecs.message = "assets/entities/playerDefault.json";
            this->_buttonVector[EnterLobbyButtonsPlayerJson] = {
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
            // New lobby button
            Rengine::Graphics::SpriteSpecs newSpecs;

            newSpecs.type = Rengine::Graphics::SpriteTypeRectangle;
            newSpecs.shapeData.specifics.rectangleSize = {300, 100};
            newSpecs.color = {250, 50, 0};
            this->_newLobbyButton = Rengine::Graphics::GraphicManagerSingletone::get().createSprite(newSpecs);
            this->_newLobbyButtonPos = {(1920 / 2) - (newSpecs.shapeData.specifics.rectangleSize.x / 2), 700};
            // New lobby text
            Rengine::Graphics::TextSpecs newText;

            newText.message = "Create a room";
            newText.characterSize = 36;
            newText.letterSpacing += 2;
            newText.fontPath = "assets/fonts/arial_narrow_7.ttf";
            this->_newLobbyButtonText = Rengine::Graphics::GraphicManagerSingletone::get().createText(newText);
        }

}  // namespace RType
