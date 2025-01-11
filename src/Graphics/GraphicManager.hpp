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
                * @brief Create the graphic manager and the window.
                */
                GraphicManager();
                ~GraphicManager(void) = default;
                /**
                * @fn createWindow
                * @param windowSpecs The specs of the to-be-created window
                * @brief Create the window following the specs.
                */
                void createWindow(const WindowSpecs& windowSpecs = WindowSpecs());
                /**
                * @fn getWindow
                * @brief Return a reference to the window.
                * If the window wasn't created previously, creates it with default settings
                */
                std::shared_ptr<AWindow>& getWindow(void) noexcept;
                /**
                * @fn createSprite
                * @param spriteSpecs The specs of the new sprite.
                * @brief Create a new sprite following the spriteSpecs,
                * this sprite is sure to be usable with the window returned by this->getWindow().
                * Note: this sprite is not stored in the GraphicManager.
                * Note: If a window wasn't created previously, creates it with default settings
                */
                std::shared_ptr<ASprite> createSprite(const SpriteSpecs& spriteSpecs);

            private:
                std::shared_ptr<AWindow> _window = nullptr;

        };  // class GraphicManager

        /**
        * @addtogroup Rengine::Graphics
        * @namespace Graphics
        * @class GraphicManagerSingletone
        * @brief The singletone class for GraphicManager.
        */
        class GraphicManagerSingletone {
            public:
                GraphicManagerSingletone(void) = default;
                ~GraphicManagerSingletone(void) = default;
                /**
                * @fn getSingletone
                * @return A static reference to the same GraphicManager object.
                * @brief Return a reference to the static GraphicManager
                */
                static GraphicManager& get(void) noexcept {
                    static GraphicManager manager;

                    return manager;
                }
        };  // class GraphicManagerSingletone

    }  // namespace Rengine
}  // namespace Graphics
#endif
