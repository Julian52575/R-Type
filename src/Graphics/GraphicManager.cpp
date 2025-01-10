//
#include <cstddef>
#include <memory>

#include "GraphicManager.hpp"
#include "AWindow.hpp"

#ifdef RENGINEGRAPHICS_USELIB_SFML
    #include "SFML/SFMLWindow.hpp"
    #include "SFML/SFMLSprite.hpp"
    #include <SFML/Window/VideoMode.hpp>
#endif

#ifdef RENGINEGRAPHICS_USELIB_SDL
    #warning "SDL not implemented yet."
    #include "./SDL/SDLWindow.hpp"
#endif

namespace Rengine {
    namespace Graphics {

        GraphicManager::GraphicManager(void)
        {
        }

        void GraphicManager::createWindow(const WindowSpecs& windowSpecs)
        {
            if (this->_window != nullptr) {
                this->_window.reset();
            }

            // SFML
        #ifdef RENGINEGRAPHICS_USELIB_SFML
            this->_window =
                std::make_unique<Rengine::Graphics::SFMLWindow>(windowSpecs);
            return;
        #endif

            // SDL
        #ifdef RENGINEGRAPHICS_USELIB_SDL
            this->_window = std::make_shared<Rengine::Graphics::SDLWindow>();
            return;
        #endif
            throw GraphicManagerException("No graphical library set. Check your rengine build.");
        }

        std::shared_ptr<AWindow>& GraphicManager::getWindow(void) noexcept
        {
            if (this->_window == nullptr) {
                this->createWindow();
            }
            return this->_window;
        }

        std::shared_ptr<ASprite> GraphicManager::createSprite(const SpriteSpecs& spriteSpecs)
        {
            // SFML
        #ifdef RENGINEGRAPHICS_USELIB_SFML
            return std::make_shared<Rengine::Graphics::SFMLSprite>(spriteSpecs, this->_window->getElapsedTimeMicroseconds());
        #endif

            // SDL
        #ifdef RENGINEGRAPHICS_USELIB_SDL
            ;
        #endif
            throw GraphicManagerException("No graphical library set. Check your rengine build.");
        }

    }  // namespace Rengine
}  // namespace Graphics
