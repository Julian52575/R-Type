//

#include <cstddef>
#include <exception>
#include <rengine/src/Clock/Clock.hpp>
#include <rengine/src/Graphics/GraphicManager.hpp>
#include <rengine/src/Graphics/SoundSpecs.hpp>
#include <rengine/src/Graphics/SpriteSpecs.hpp>
#include <rengine/src/Graphics/TextSpecs.hpp>
#include <rengine/src/Graphics/UserInputManager.hpp>

#include "TitleScreenScene.hpp"
#include "src/Config/ImageConfigResolver.hpp"
#include "src/State/Menu/Scenes.hpp"

namespace RType {

    void TitleScreenScene::unload(void)
    {
        if (this->_backgroundMusic != nullptr) {
            this->_backgroundMusic->pause();
        }
    }

    void TitleScreenScene::reload(void)
    {
        if (this->_backgroundMusic != nullptr) {
            this->_backgroundMusic->play();
        }
    }
    void TitleScreenScene::display(void)
    {
        Rengine::Graphics::GraphicManagerSingletone::get().addToRender(this->_logo,
            {450,
            75}
        );
        if (this->_backgroundMusic != nullptr && this->_backgroundMusic->isPlaying() == false) {
            this->_backgroundMusic->play();
        }
        this->_textDeltatime += Rengine::Clock::getElapsedTime();
        if (this->_textDeltatime > 2.0f) {
            Rengine::Graphics::GraphicManagerSingletone::get().addToRender(this->_enterText,
                {700,
                550}
            );
            Rengine::Graphics::GraphicManagerSingletone::get().addToRender(this->_tabText,
                {380,
                750}
            );
            if (this->_textDeltatime > 2.0f * 2) {
                this->_textDeltatime = 0.0f;
            }
        }
    }

    MenuScenes TitleScreenScene::handleInputs(void)
    {
        for (auto& it : Rengine::Graphics::GraphicManagerSingletone::get().getWindow()->getInputManager()) {
            switch (it.type) {
                case Rengine::Graphics::UserInputTypeKeyboardSpecialPressed:
                    switch (it.data.keyboardSpecial) {
                        case Rengine::Graphics::UserInputKeyboardSpecialESCAPE:
                            return static_cast<MenuScenes>(this->_sceneIndex - 1);

                        case Rengine::Graphics::UserInputKeyboardSpecialENTER:
                            return MenuScenesExitToGame;

                        case Rengine::Graphics::UserInputKeyboardSpecialTAB:
                            return MenuScenesEnterLobbyInfo;

                        default:
                            break;
                    }  // switch it.data.keyboardSpecial

                default:
                    break;
            }  // switch it.type
        }

        return MenuScenes::MenuScenesTitleScreen;
    }

    void TitleScreenScene::initGraphics(void)
    {
        // Sparking zero BGM
        Rengine::Graphics::SoundSpecs music;

        music.soundPath = "assets/musics/DRAGON BALL Sparking! Zero - Main Menu Theme (Official).mp3";
        music.loop = true;
        try {
            this->_backgroundMusic = Rengine::Graphics::GraphicManagerSingletone::get().createSound(music);
        } catch (std::exception& e) {
            std::cout << "Cannot load :" << music.soundPath << " : " << e.what() << std::endl;
        }
        // Logo
        const RType::Config::ImageConfig& logo = RType::Config::ImageConfigResolverSingletone::get().get("assets/images/RTypeLogo.json");

        this->_logo = Rengine::Graphics::GraphicManagerSingletone::get().createSprite(logo.getSpecs());
        // Space Text
        Rengine::Graphics::TextSpecs text;

        text.fontPath = "assets/fonts/ci-gamedev.ttf";
        text.message = "Press enter";
        text.characterSize = 42;
        text.outlineThickness = 1;
        text.outlineColor = {255, 255, 255};
        text.letterSpacing = 5;
        text.color = {255, 255, 255};
        this->_enterText = Rengine::Graphics::GraphicManagerSingletone::get().createText(text);
        text.message = "Press tab for online player";
        this->_tabText = Rengine::Graphics::GraphicManagerSingletone::get().createText(text);
    }

}  // namespace RType
