//

#include "CreateLobbyScene.hpp"
#include "src/Config/ImageConfigResolver.hpp"
#include "src/State/AScene.hpp"
#include "src/State/Menu/Scenes.hpp"
#include <math.h>
#include <rengine/src/Graphics/GraphicManager.hpp>
#include <rengine/src/Graphics/SoundSpecs.hpp>
#include <rengine/src/Graphics/SpriteSpecs.hpp>
#include <rengine/src/Graphics/TextSpecs.hpp>
#include <rengine/src/Graphics/UserInputManager.hpp>

namespace RType {

    void CreateLobbyScene::unload(void)
    {
        this->_backgroundMusic->reset();
        this->_isLoaded = false;
    }
    void CreateLobbyScene::reload(void)
    {
        this->_backgroundMusic->play();
        this->_isLoaded = true;
    }
    void CreateLobbyScene::display(void)
    {
        Rengine::Graphics::GraphicManagerSingletone::get().addToRender(this->_background, {0, 0});
        Rengine::Graphics::GraphicManagerSingletone::get().addToRender(this->_title,
            {1920 / 2 - 150,
            50}
        );
        Rengine::Graphics::GraphicManagerSingletone::get().addToRender(this->_underTitle,
            {1920 / 2 - 100,
            150}
        );
        Rengine::Graphics::GraphicManagerSingletone::get().addToRender(this->_textboxShadox,
                {700,
                450}
        );
        Rengine::Graphics::GraphicManagerSingletone::get().addToRender(this->_textbox,
                {780,
                450}
        );
        Rengine::Graphics::GraphicManagerSingletone::get().addToRender(this->_enter,
                {1450,
                375}
        );
    }
    MenuScenes CreateLobbyScene::handleInputs(void)
    {
        bool updated = false;
        std::string copy = this->_textbox->getText();

        for (auto& it : Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager()) {
            switch (it.type) {

                case Rengine::Graphics::UserInputTypeKeyboardSpecialPressed:
                    if (it.data.keyboardSpecial == Rengine::Graphics::UserInputKeyboardSpecialENTER) {
                        this->copyTexbox();
                        return MenuScenes::MenuScenesExitToLobby;
                    }
                    if (it.data.keyboardSpecial == Rengine::Graphics::UserInputKeyboardSpecialESCAPE) {
                        this->_lobbyName = "";
                        return MenuScenes::MenuScenesEnterLobbyInfo;
                    }
                    if (it.data.keyboardSpecial == Rengine::Graphics::UserInputKeyboardSpecialBACKSPACE
                    && copy.size() > 0) {
                        copy.resize(copy.size() - 1);
                        updated = true;
                    }
                    break;

                case Rengine::Graphics::UserInputTypeKeyboardCharPressed:
                    if (copy.size() >= 16) {
                        break;
                    }
                    copy += it.data.keyboardChar;
                    updated = true;
                    break;

                default:
                    break;
            }  // switch it.type
        }  // for it
        if (updated == true) {
            this->_textbox->setText(copy);
        }
        return this->_sceneIndex;
    }
    void CreateLobbyScene::copyTexbox(void)
    {
        this->_lobbyName = this->_textbox->getText();
    }

    void CreateLobbyScene::initGraphics(void)
    {
        const Rengine::Graphics::SpriteSpecs& specs = RType::Config::ImageConfigResolverSingletone::get().get("assets/images/menu/nasaStarship.json").getSpecs();

        this->_background = Rengine::Graphics::GraphicManagerSingletone::get().createSprite(specs);
        Rengine::Graphics::TextSpecs text;

        text.fontPath = "assets/fonts/arial_narrow_7.ttf";
        text.message = "Create a lobby";
        text.characterSize = 84;
        text.style = Rengine::Graphics::TextStyleUnderline;
        this->_title = Rengine::Graphics::GraphicManagerSingletone::get().createText(text);
        text.message = "Enter a name";
        text.characterSize = 64;
        text.style = Rengine::Graphics::TextStyleRegular;
        this->_underTitle = Rengine::Graphics::GraphicManagerSingletone::get().createText(text);
        text.message = "";
        text.letterSpacing += 1;
        this->_textbox = Rengine::Graphics::GraphicManagerSingletone::get().createText(text);

        Rengine::Graphics::SpriteSpecs shadow;

        shadow.type = Rengine::Graphics::SpriteTypeRectangle;
        shadow.color = {0, 0, 0};
        shadow.opacity = 150;
        shadow.shapeData.specifics.rectangleSize = {600, 100};
        this->_textboxShadox = Rengine::Graphics::GraphicManagerSingletone::get().createSprite(shadow);

        const Rengine::Graphics::SpriteSpecs& enter = RType::Config::ImageConfigResolverSingletone::get().get("assets/images/menu/enter.json").getSpecs();

        this->_enter = Rengine::Graphics::GraphicManagerSingletone::get().createSprite(enter);

        Rengine::Graphics::SoundSpecs worlds;

        worlds.soundPath = "assets/musics/worlds.ogg";
        worlds.loop = true;

        this->_backgroundMusic = Rengine::Graphics::GraphicManagerSingletone::get().createSound(worlds);
    }

}  // namespace RType
