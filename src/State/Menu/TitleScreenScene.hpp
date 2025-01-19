//
#pragma once

#include <cstddef>
#include <rengine/RengineGraphics.hpp>
#include <rengine/src/Graphics/ASound.hpp>
#include <rengine/src/Graphics/ASprite.hpp>
#include <rengine/src/Graphics/AText.hpp>
#include <rengine/src/Rng/Rng.hpp>

#include "src/State/AScene.hpp"
#include "src/State/Menu/Scenes.hpp"

namespace RType {

    class TitleScreenScene : public AScene<MenuScenes> {
        public:
            TitleScreenScene(void)
                : AScene(MenuScenesTitleScreen)  // Constructor in hpp for AScene(sceneIndex)
            {
                this->initGraphics();
            }
            ~TitleScreenScene(void) = default;
            void unload(void);
            void reload(void);
            void display(void);
            MenuScenes handleInputs(void);

        private:
            void initGraphics(void);

        private:
            std::shared_ptr<Rengine::Graphics::ASprite> _logo = nullptr;
            std::shared_ptr<Rengine::Graphics::ASound> _backgroundMusic = nullptr;
            std::shared_ptr<Rengine::Graphics::ASprite> _background = nullptr;
            float _textDeltatime = 2.0f;
            std::shared_ptr<Rengine::Graphics::AText> _enterText = nullptr;
            std::shared_ptr<Rengine::Graphics::AText> _spaceText = nullptr;
    };

}  // namespace RType
