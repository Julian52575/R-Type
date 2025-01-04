//
#ifndef _SRC_GRAPHICS_SDL_SDLWINDOW_HPP_
#define _SRC_GRAPHICS_SDL_SDLWINDOW_HPP_

#include <optional>

#include "../IWindow.hpp"

namespace Rengine {
    namespace Graphics {

#error SDL should not be compiled now. This header exists only as a compilation test.
        class SDLWindow : public IWindow {
            public:
                SDLWindow(void) = default;
                ~SDLWindow(void) = default;
                void render(void);
                void addSpriteToRender(Rengine::Graphics::ASprite& sprite, const Rengine::Graphics::vector2D<float>& position);

            private:
        };

    }  // namespace Rengine
}  // namespace Graphics

#endif
