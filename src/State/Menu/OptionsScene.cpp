//
#include <memory>
#include <rengine/src/Clock/Clock.hpp>
#include <rengine/src/Graphics/AWindow.hpp>
#include <rengine/src/Graphics/GraphicManager.hpp>
#include <rengine/src/Graphics/SpriteSpecs.hpp>
#include <rengine/src/Graphics/TextSpecs.hpp>
#include <rengine/src/Graphics/UserInputManager.hpp>

#include "src/Config/ImageConfig.hpp"
#include "src/Network/EntityAction.hpp"
#include "src/State/AScene.hpp"
#include "src/State/Menu/OptionsScene.hpp"
#include "src/State/Menu/Scenes.hpp"
#include "src/Game/AccessibilitySettings.hpp"
#include "src/Config/ImageConfigResolver.hpp"
#include "src/Components/Action.hpp"

namespace RType {

        // Called when leaving this scene
        // mainly used for music
    void OptionsScene::unload(void)
    {
        this->_deltatime = 0;
        this->_isLoaded = false;
    }
        // Called when coming back to this scene
        // mainly used for music
    void OptionsScene::reload(void)
    {
        this->_isLoaded = true;
    }

        // Called each frame
    void OptionsScene::display(void)
    {
        Rengine::Graphics::GraphicManager& manager = Rengine::Graphics::GraphicManagerSingletone::get();

        this->_deltatime += Rengine::Clock::getElapsedTime();
        manager.addToRender(this->_colorWheel, {.x= 100, .y = 100});
        manager.addToRender(this->_rectangle, {.x= 200, .y = 300});
        manager.addToRender(this->_text, {800, 50});
    }
        // Called each frame
    MenuScenes OptionsScene::handleInputs(void)
    {
        Rengine::Graphics::UserInputManager& manager = Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager();

        for (auto& it : manager) {
            if (it.type == Rengine::Graphics::UserInputTypeKeyboardSpecialPressed) {
                if (it.data.keyboardSpecial == Rengine::Graphics::UserInputKeyboardSpecialESCAPE) {
                    return MenuScenesTitleScreen;  // go back to title screen on escape
                }
                if (it.data.keyboardSpecial == Rengine::Graphics::UserInputKeyboardSpecialArrowDOWN) {
                    std::cout << "Pressed down" << std::endl;
                }
            }  // if SpecialPressed
            if (it.type == Rengine::Graphics::UserInputTypeKeyboardCharPressed && it.data.keyboardChar == 'd') {
                this->loadNextShader();
            }
            if (it.type == Rengine::Graphics::UserInputTypeMouseLeftClick) {
                std::cout << "Click at " << it.data.mousePosition << std::endl;
            }  // if LeftClick
        }  // for it

        // Use static functions from RType::Components::Action to change the player input
        // RType::Components::Action::changePlayerInput(Rengine::Graphics::UserInput newInput, Network::EntityActionType resultingAction);
        // RType::Components::Action::getPlayerNeededInput(Network::EntityActionType outcome);
        // Example: RType::Components::Action::changePlayerInput(
        //          {
        //          Rengine::Graphics::UserInputTypeKeyboardSpecial,
        //          Rengine::Graphics::UserInputKeyboardSpecialENTER
        //          },
        //          Network::EntityActionType::EntityActionTypeShoot1);
        //          To bind the shoot1 action to the enter key.
        return MenuScenesOptions;
    }
    void OptionsScene::loadNextShader(void)
    {
        std::shared_ptr<Rengine::Graphics::AWindow> window = Rengine::Graphics::GraphicManagerSingletone::get().getWindow();

        this->_shaderIndex = (this->_shaderIndex + 1) % this->_shaderVector.size();
        window->setShader("", this->_shaderVector[this->_shaderIndex]);
        std::cout << "Loaded shader : " << this->_shaderVector[this->_shaderIndex] << " at index " << this->_shaderIndex << std::endl;  //
    }

    void OptionsScene::initGraphics(void)
    {
        // example image
        RType::Config::ImageConfig spriteImage = RType::Config::ImageConfigResolverSingletone::get().get("assets/images/menu/colorWheel.json");

        this->_colorWheel = Rengine::Graphics::GraphicManagerSingletone::get().createSprite(spriteImage.getSpecs());
        // example text
        Rengine::Graphics::TextSpecs textSpecs;

        textSpecs.fontPath = "assets/fonts/arial_narrow_7.ttf";
        textSpecs.message = "Options (press d for shader)";
        textSpecs.letterSpacing = 3;
        textSpecs.style = Rengine::Graphics::TextStyle::TextStyleBold | Rengine::Graphics::TextStyle::TextStyleUnderline;
        this->_text = Rengine::Graphics::GraphicManagerSingletone::get().createText(textSpecs);
        // example rectangle
        Rengine::Graphics::SpriteSpecs rectSpecs;

        rectSpecs.type = Rengine::Graphics::SpriteType::SpriteTypeRectangle;
        rectSpecs.color = {255, 0, 0};
        rectSpecs.shapeData.specifics.rectangleSize = {.x = 120, .y = 150};
        this->_rectangle = Rengine::Graphics::GraphicManagerSingletone::get().createSprite(rectSpecs);
    }
    void OptionsScene::initShaders(void)
    {
        uint64_t i = 0;

        // Parsing shaders file
        for (auto file : std::filesystem::directory_iterator("assets/shaders/")) {
            // Found default shader
            if (file.path().string().find("default") != std::string::npos) {
                this->_shaderIndex = i;
            }
            this->_shaderVector.push_back(file.path().string());
            i++;
        }
    }

}  // namespace RType
