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
#include "WindowSpecs.hpp"

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
        /**
        * @addtogroup Rengine::Graphics
        * @namespace Graphics
        * @class GraphicManager
        * @brief The entrypoint class for managing graphics in the rengine.
        */
        class GraphicManager {
            public:
                /**
                * @fn GraphicManager
                * @param windowSpecs The specs of the to-be-created window
                * @brief Create the graphic manager and the window.
                */
                GraphicManager(const WindowSpecs& windowSpecs);
                ~GraphicManager(void) = default;
                /**
                * @fn getWindow
                * @brief Return a reference to the window.
                */
                std::shared_ptr<AWindow>& getWindow(void) noexcept;
                /**
                * @fn createSprite
                * @param spriteSpecs The specs of the new sprite.
                * @brief Create a new sprite following the spriteSpecs,
                * this sprite is sure to be usable with the window returned by this->getWindow().
                * Note: this sprite is not stored in the GraphicManager.
                */
                std::shared_ptr<ASprite> createSprite(const SpriteSpecs& spriteSpecs);

            private:
                std::shared_ptr<AWindow> _window = nullptr;

        };  // class GraphicManager

    }  // namespace Rengine
}  // namespace Graphics
#endif
