//
#pragma once

#include "src/State/AScene.hpp"
#include "src/State/Menu/Scenes.hpp"
#include "src/Game/AccessibilitySettings.hpp"
#include <memory>
#include <rengine/src/Graphics/ASound.hpp>
#include <rengine/src/Graphics/ASprite.hpp>
#include <rengine/src/Graphics/AText.hpp>

namespace RType {


    class OptionsScene : public AScene<MenuScenes> {
        public:
            OptionsScene(AccessibilitySettings& accessibilitySettings)
                : AScene(MenuScenesOptions), _accessibilitySettings(accessibilitySettings)  // Constructor in hpp for AScene(sceneIndex)
            {
                this->initGraphics();
            }
            ~OptionsScene(void) = default;
            void unload(void);
            void reload(void);
            void display(void);
            MenuScenes handleInputs(void);

        private:
            void initGraphics(void);

        private:
            float _deltatime = 0.0f;
            std::shared_ptr<Rengine::Graphics::AText> _text;
            std::shared_ptr<Rengine::Graphics::ASprite> _sprite;
            std::shared_ptr<Rengine::Graphics::ASprite> _rectangle;
            AccessibilitySettings& _accessibilitySettings;
    };

}  // namespace RType
