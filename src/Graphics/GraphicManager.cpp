//
#include <cstddef>
#include <memory>

#include "ASound.hpp"
#include "GraphicManager.hpp"
#include "AWindow.hpp"
#include "SFML/SFMLSound.hpp"
#include "SpriteSpecs.hpp"
#include "TextSpecs.hpp"
#include "SoundSpecs.hpp"

#ifdef __RENGINEGRAPHICS_USELIB_SFML
    #include "SFML/SFMLWindow.hpp"
    #include "SFML/SFMLSprite.hpp"
    #include "SFML/SFMLText.hpp"

    #include <SFML/Window/VideoMode.hpp>
#endif

#ifdef __RENGINEGRAPHICS_USELIB_SDL
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
        #ifdef __RENGINEGRAPHICS_USELIB_SFML
            this->_window =
                std::make_unique<Rengine::Graphics::SFMLWindow>(windowSpecs);
            return;
        #endif

            // SDL
        #ifdef __RENGINEGRAPHICS_USELIB_SDL
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
            if (this->_window == nullptr) {
                this->createWindow();
            }
            // SFML
        #ifdef __RENGINEGRAPHICS_USELIB_SFML
            return std::make_shared<Rengine::Graphics::SFMLSprite>(spriteSpecs, this->_window->getElapsedTimeMicroseconds());
        #endif

            // SDL
        #ifdef __RENGINEGRAPHICS_USELIB_SDL
            {}
        #endif
            throw GraphicManagerException("No graphical library set. Check your rengine build.");
        }

        std::shared_ptr<AText> GraphicManager::createText(const TextSpecs& textSpecs)
        {
        #ifdef __RENGINEGRAPHICS_USELIB_SFML
            return std::make_shared<Rengine::Graphics::SFMLText>(textSpecs);
        #endif
        }

        std::shared_ptr<ASound> GraphicManager::createSound(const SoundSpecs& musicSpecs)
        {
        #ifdef __RENGINEGRAPHICS_USELIB_SFML
            return std::make_shared<Rengine::Graphics::SFMLSound>(musicSpecs);
        #endif
        }

        void GraphicManager::addToRender(const std::shared_ptr<Rengine::Graphics::ASprite>& sprite,
                const Rengine::Graphics::vector2D<float>& position, bool updateAnimationFrame)
        {
            this->_window->addSpriteToRender(sprite, position, updateAnimationFrame);
        }

        void GraphicManager::addToRender(const std::shared_ptr<Rengine::Graphics::AText>& text,
                const Rengine::Graphics::vector2D<float>& position)
        {
            this->_window->addTextToRender(text, position);
        }

    }  // namespace Graphics
}  // namespace Rengine
