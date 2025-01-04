//
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <exception>

#include "SFMLSprite.hpp"
#include "../ASprite.hpp"
#include "../SpriteSpecs.hpp"

namespace Rengine {
    namespace Graphics {

            SFMLSprite::SFMLSprite(const SpriteSpecs& spriteSpecs) : ASprite(spriteSpecs)
            {
                try {
                    this->_texture.loadFromFile(this->_spriteSpecs.texturePath);
                } catch (std::exception& e) {
                    std::string msg = e.what();
                    throw SpriteException(msg);
                }
                this->setSpriteSpecs(spriteSpecs);
            }
            void SFMLSprite::setSpriteSpecs(const Rengine::Graphics::SpriteSpecs& spriteSpecs)
            {
                this->_spriteSpecs = spriteSpecs;
                sf::IntRect rect;

                rect.left = this->_spriteSpecs.animation.frameRectXY.x;
                rect.top = this->_spriteSpecs.animation.frameRectXY.y;
                rect.width = this->_spriteSpecs.animation.frameRectWidthHeight.x;
                rect.height = this->_spriteSpecs.animation.frameRectWidthHeight.y;
                this->_sprite = sf::Sprite(this->_texture, rect);
                this->_sprite.setScale({this->_spriteSpecs.textureScale.x, this->_spriteSpecs.textureScale.y});
            }

            void SFMLSprite::advanceFrame(const int16_t frameCount)
            {
                if (frameCount == 0) {
                    return;
                }
                sf::IntRect rect;

                this->_currentFrame += frameCount;
                this->_currentFrame %= this->_spriteSpecs.animation.frameCount;
                rect.left = this->_spriteSpecs.animation.frameRectXY.x + (this->_spriteSpecs.animation.frameDisplacement.x * this->_currentFrame);
                rect.top = this->_spriteSpecs.animation.frameRectXY.y + (this->_spriteSpecs.animation.frameDisplacement.y * this->_currentFrame);
                rect.width = this->_spriteSpecs.animation.frameRectWidthHeight.x;
                rect.height = this->_spriteSpecs.animation.frameRectWidthHeight.y;
                this->_sprite.setTextureRect(rect);
            }
            sf::Sprite& SFMLSprite::getSfSprite(void) noexcept
            {
                return this->_sprite;
            }
            const sf::Sprite& SFMLSprite::getSfSprite(void) const noexcept
            {
                return this->_sprite;
            }

    }  // namespace Rengine
}  // namespace Graphics
