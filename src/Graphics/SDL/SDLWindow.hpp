//
#ifndef _SRC_GRAPHICS_SDL_SDLWINDOW_HPP_
#define _SRC_GRAPHICS_SDL_SDLWINDOW_HPP_

#include <memory>
#include <optional>

#include "../AWindow.hpp"
#include "../WindowSpecs.hpp"

namespace Rengine {
    namespace Graphics {

#error SDL should not be compiled now. This header exists only as a compilation test.
        class SDLWindow : public AWindow {
            public:
                SDLWindow(const Rengine::Graphics::WindowSpecs& specs) : AWindow(specs) {};
                ~SDLWindow(void) = default;
                void render(void);
                void addSpriteToRender(const std::shared_ptr<Rengine::Graphics::ASprite>& sprite, const Rengine::Graphics::vector2D<float>& position);

            private:
        };

    }  // namespace Rengine
}  // namespace Graphics

#endif
