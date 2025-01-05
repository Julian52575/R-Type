//
#ifndef _SRC_GRAPHICS_GRAPHICMANAGER_HPP_
#define _SRC_GRAPHICS_GRAPHICMANAGER_HPP_
#include <memory>

// SFML Includes if define
#ifdef RENGINEGRAPHICS_USELIB_SFML
    #include "./SFML/SFMLWindow.hpp"
    #include "./SFML/SFMLSprite.hpp"
#endif
// SFL Includes if define
#ifdef RENGINEGRAPHICS_USELIB_SDL
    #warning "SDL not implemented yet."
    #include "./SDL/SDLWindow.hpp"
#endif

#include "AWindow.hpp"
#include "ASprite.hpp"
#include "Vector.hpp"
#include "SpriteSpecs.hpp"

namespace Rengine {
    namespace Graphics {

        class GraphicManagerException : public std::exception {
            public:
                GraphicManagerException(const std::string& msg) : _msg("Rengine::Graphics::GraphicManager: " + msg) {};
                const char *what() const noexcept
                {
                    return this->_msg.c_str();
                };

            private:
                std::string _msg;
        };

        class GraphicManager {
            public:
                GraphicManager(Rengine::Graphics::vector2D<uint16_t> windowSize, const std::string& windowTitle);
                ~GraphicManager(void) = default;
                std::shared_ptr<AWindow>& getWindow(void) noexcept;
                std::shared_ptr<ASprite> createSprite(const SpriteSpecs& spriteSpecs);

            private:
                std::shared_ptr<AWindow> _window = nullptr;

        };  // class GraphicManager

    }  // namespace Rengine
}  // namespace Graphics
#endif
