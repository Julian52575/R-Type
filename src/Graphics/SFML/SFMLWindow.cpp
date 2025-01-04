//
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <exception>

#include "SFMLWindow.hpp"
#include "SFMLSprite.hpp"

namespace Rengine {
    namespace Graphics {

        SFMLWindow::SFMLWindow(const sf::VideoMode& videoMode, const std::string& windowTitle) : _renderWindow(videoMode, windowTitle)
        {
        }
        void SFMLWindow::render(void)
        {
            this->_renderWindow.display();
            this->_renderWindow.clear();
        }
        void SFMLWindow::addSpriteToRender(Rengine::Graphics::ASprite& sprite, const Rengine::Graphics::vector2D<float>& position)
        {
            try {
                Rengine::Graphics::SFMLSprite& sfmlSprite = (SFMLSprite&) sprite;
                sf::Sprite& sfSprite = sfmlSprite.getSfSprite();
                sf::Vector2f posVector = {position.x, position.y};

                sfSprite.setPosition(posVector);
                this->_renderWindow.draw(sfSprite);
            } catch (std::exception& e) {
                std::string msg = e.what();
                throw WindowException(msg);
            }
        }
        bool SFMLWindow::isOpen(void)
        {
            return this->_renderWindow.isOpen();
        }
        void SFMLWindow::close(void)
        {
            this->_renderWindow.close();
        }

    }  // namespace Rengine
}  // namespace Graphics
