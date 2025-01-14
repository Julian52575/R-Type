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

namespace RType {

        MenuState::MenuState(Rengine::ECS& ecs) : AState(ecs)
        {
                this->registerComponents();
                this->makeButtons();
                this->_currentScene = RTypeMenuScenes::RTypeMenuScenesButtonDisplay;

                Rengine::Graphics::SoundSpecs specs;

                specs.soundPath = "assets/musics/starforx64_training_mode.mp3";
                specs.loop = true;
                this->_backgroundMusic = Rengine::Graphics::GraphicManagerSingletone::get().createSound(specs);
                this->_backgroundMusic->play();
        }
        void MenuState::registerComponents(void)
        {
        }
            /*
            * @fn run
            * @return State The requested next state of the game.
            * @brief Run the program in it's current state.
            */
            State MenuState::run(void)
            {
                // WIP
                float count = 1;
                float yDiff = 100;

#warning Use sceneManger better
                if (this->_currentScene == RTypeMenuScenes::RTypeMenuScenesButtonDisplay) {
                    this->handleInput();
                    // _cursor
                    Rengine::Graphics::GraphicManagerSingletone::get().addToRender(this->_cursor, {50, yDiff * (int) (this->_currentIndex + 1) + 15 });
                    // Display button and input
                    for (auto it : this->_buttonVector) {
                        const std::string& currentField = it.first->getText();

                        Rengine::Graphics::GraphicManagerSingletone::get().addToRender(it.first, {50, yDiff * count});
                        Rengine::Graphics::GraphicManagerSingletone::get().addToRender(it.second, {550, yDiff * count});
                        count += 1;
                    }  // it : this->_buttonVector
                } else if (this->_currentScene == RTypeMenuScenes::RTypeMenuScenesLoadLobbyInfoAndExit) {
                    this->_lobbyInfo.serverIp = this->_buttonVector[MenuStateButtonsIp].second->getText();
                    try {
                        this->_lobbyInfo.port = std::stoi(this->_buttonVector[MenuStateButtonsPort].second->getText());
                    } catch (std::exception& e) {
                        std::cout << "Exception '" << e.what() << "' when converting port input to int. Staying on menu." << std::endl;
                        this->_currentScene = RTypeMenuScenesButtonDisplay;
                        #warning Use sceneManger better 2
                        return StateMenu;
                    }
                    this->_backgroundMusic->reset();
                    this->_currentScene = RTypeMenuScenes::RTypeMenuScenesButtonDisplay;
                    return StateLobby;
                }
                return StateMenu;
            }
            const LobbyInfo& MenuState::getLobbyInfo(void) const noexcept
            {
                return this->_lobbyInfo;
            }

            void MenuState::handleInput(void)
            {
                Rengine::Graphics::UserInputManager inputManager = Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager();
                std::string currentTextCopy = this->_buttonVector[this->_currentIndex % MenuStateButtonsMax].second->getText();
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
                                this->_currentIndex = static_cast<MenuStateButtons>((this->_currentIndex + 1) % MenuStateButtonsMax);
                                break;

                            case Rengine::Graphics::UserInputKeyboardSpecialENTER:
                                this->_currentScene = RTypeMenuScenesLoadLobbyInfoAndExit;
                                #warning Use SceneManager better 3
                                break;

                            default:
                                break;
                        }
                    }
                }  // for it
                if (textUpdate == true) {
                    this->_buttonVector[this->_currentIndex % MenuStateButtonsMax].second->setText(currentTextCopy);
                }
            }


            void MenuState::makeButtons(void)
            {
                this->_buttonVector.resize(MenuStateButtons::MenuStateButtonsMax);
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
                this->_buttonVector[MenuStateButtonsIp] = {
                        Rengine::Graphics::GraphicManagerSingletone::get().createText(specs),
                        Rengine::Graphics::GraphicManagerSingletone::get().createText(inputSpecs)
                };

                specs.color = {255, 0, 0};
                specs.message = "Port :";
                inputSpecs.message = "4242";
                this->_buttonVector[MenuStateButtonsPort] = {
                    Rengine::Graphics::GraphicManagerSingletone::get().createText(specs),
                    Rengine::Graphics::GraphicManagerSingletone::get().createText(inputSpecs)
                };
                specs.color = {0, 255, 0};
                specs.message = "Player json :";
                inputSpecs.message = "assets/entities/playerDefault.json";
                this->_buttonVector[MenuStateButtonsPlayerJson] = {
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
                this->_cursor = manager.createSprite(spriteSpecs);
            }
}  // namespace RType
