//
#ifndef _SRC_GRAPHICS_SFML_SFMLWINDOW_HPP_
#define _SRC_GRAPHICS_SFML_SFMLWINDOW_HPP_
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <optional>
#include <memory>

#include "../IWindow.hpp"
#include "SFMLSprite.hpp"

namespace Rengine {
    namespace Graphics {

        class SFMLWindow : public IWindow {
            public:
                SFMLWindow(const sf::VideoMode& videoMode, const std::string& windowTitle);
                ~SFMLWindow(void) = default;
                void render(void);
                void addSpriteToRender(const std::shared_ptr<Rengine::Graphics::ASprite>& sprite, const Rengine::Graphics::vector2D<float>& position);
                bool isOpen(void);
                void close(void);

            private:
                sf::RenderWindow _renderWindow;
        };

    }  // namespace Rengine
}  // namespace Graphics

#endif
