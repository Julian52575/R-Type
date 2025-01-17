//
#include <rengine/src/Clock/Clock.hpp>
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
    }
        // Called when coming back to this scene
        // mainly used for music
    void OptionsScene::reload(void)
    {
    }

        // Called each frame
    void OptionsScene::display(void)
    {
        Rengine::Graphics::GraphicManager& manager = Rengine::Graphics::GraphicManagerSingletone::get();

        this->_deltatime += Rengine::Clock::getElapsedTime();
        manager.addToRender(this->_sprite, {.x= 100, .y = 100});
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

    void OptionsScene::initGraphics(void)
    {
        // example image
        RType::Config::ImageConfig spriteImage = RType::Config::ImageConfigResolverSingletone::get().get("assets/images/playerDefault.json");

        this->_sprite = Rengine::Graphics::GraphicManagerSingletone::get().createSprite(spriteImage.getSpecs());
        // example text
        Rengine::Graphics::TextSpecs textSpecs;

        textSpecs.fontPath = "assets/fonts/arial_narrow_7.ttf";
        textSpecs.message = "Options";
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


}  // namespace RType
