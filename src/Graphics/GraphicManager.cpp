//
#include <memory>

#include "GraphicManager.hpp"

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

        GraphicManager::GraphicManager(GraphicManagerLibrary requestedLib,
            Rengine::Graphics::vector2D<uint16_t> windowSize, const std::string& windowTitle)
        {
            this->_requestedLib = requestedLib;
            switch (requestedLib) {

            // SFML
            #ifdef RENGINEGRAPHICS_USELIB_SFML
                case (GraphicManagerLibrarySFML):
                    this->_window =
                        std::make_unique<Rengine::Graphics::SFMLWindow>(sf::VideoMode({windowSize.x, windowSize.y}), windowTitle);
                    break;
            #endif

            // SDL
            #ifdef RENGINEGRAPHICS_USELIB_SDL
                case (GraphicManagerLibrarySDL):
                    this->_window =
                        std::make_unique<Rengine::Graphics::SDLWindow>();
                    break;
            #endif

                default:
                    throw GraphicManagerException("The requested library is unknow.");
            }

        }
        std::unique_ptr<IWindow>& GraphicManager::getWindow(void) noexcept
        {
            return this->_window;
        }
        std::shared_ptr<ASprite> GraphicManager::createSprite(const SpriteSpecs& spriteSpecs)
        {
            switch (this->_requestedLib) {

            // SFML
            #ifdef RENGINEGRAPHICS_USELIB_SFML
                case (GraphicManagerLibrarySFML):
                    return std::make_shared<Rengine::Graphics::SFMLSprite>(spriteSpecs);
            #endif

            // SDL
            #ifdef RENGINEGRAPHICS_USELIB_SDL
                case (GraphicManagerLibrarySDL):
                    break;
            #endif

                default:
                    break;
            }
            throw GraphicManagerException("The requested library is unknow.");
        }

    }  // namespace Rengine
}  // namespace Graphics
