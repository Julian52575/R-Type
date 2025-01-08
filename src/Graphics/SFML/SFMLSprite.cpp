//
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <exception>

#include "SFMLSprite.hpp"
#include "../ASprite.hpp"
#include "../SpriteSpecs.hpp"

namespace Rengine {
    namespace Graphics {

            SFMLSprite::SFMLSprite(const SpriteSpecs& spriteSpecs, uint64_t creationTickMicroseconds) : ASprite(spriteSpecs, creationTickMicroseconds)
            {
                try {
                    this->_texture.loadFromFile(this->_spriteSpecs.texturePath);
                    this->_sprite.setTexture(this->_texture);
                } catch (std::exception& e) {
                    std::string msg = e.what();
                    throw SpriteException(msg);
                }
                // setSpriteSpecs
                this->_spriteSpecs = spriteSpecs;
                // scale
                this->_sprite.setScale({this->_spriteSpecs.textureScale.x, this->_spriteSpecs.textureScale.y});
                // animation
                if (this->_spriteSpecs.animation.has_value() == true) {
                    sf::IntRect rect;

                    rect.left = this->_spriteSpecs.animation->frameRectXY.x;
                    rect.top = this->_spriteSpecs.animation->frameRectXY.y;
                    rect.width = this->_spriteSpecs.animation->frameRectWidthHeight.x;
                    rect.height = this->_spriteSpecs.animation->frameRectWidthHeight.y;
                    this->_sprite.setTextureRect(rect);
                }
            }

            void SFMLSprite::advanceFrame(int16_t frameCount)
            {
                if (this->_spriteSpecs.animation.has_value() == false) {
                    return;
                }
                if (frameCount == 0) {
                    return;
                }
                this->_currentFrame += frameCount;
                this->_currentFrame %= this->_spriteSpecs.animation->frameCount;
                this->applyCurrentFrameTexture();
            }
            void SFMLSprite::advanceFrameFromTime(uint64_t currentTimeMicroseconds)
            {
                if (this->_spriteSpecs.animation.has_value() == false) {
                    return;
                }
                #define microToSeconds 1000000
                // Division by 0 check
                if (this->_spriteSpecs.animation->frameDuration == 0 || this->_spriteSpecs.animation->frameCount == 0) {
                    return;
                }
                // Current second
                double second = (double) currentTimeMicroseconds / microToSeconds;
                // Current frame since creation: how many frameDuration in current second ?
                uint64_t frameCount = second / this->_spriteSpecs.animation->frameDuration;

                this->_currentFrame = frameCount % this->_spriteSpecs.animation->frameCount;
                this->applyCurrentFrameTexture();
            }
            sf::Sprite& SFMLSprite::getSfSprite(void) noexcept
            {
                return this->_sprite;
            }
            const sf::Sprite& SFMLSprite::getSfSprite(void) const noexcept
            {
                return this->_sprite;
            }
            void SFMLSprite::applyCurrentFrameTexture(void)
            {
                if (this->_spriteSpecs.animation.has_value() == false) {
                    return;
                }
                sf::IntRect rect;

                rect.left = this->_spriteSpecs.animation->frameRectXY.x + (this->_spriteSpecs.animation->frameDisplacement.x * this->_currentFrame);
                rect.top = this->_spriteSpecs.animation->frameRectXY.y + (this->_spriteSpecs.animation->frameDisplacement.y * this->_currentFrame);
                rect.width = this->_spriteSpecs.animation->frameRectWidthHeight.x;
                rect.height = this->_spriteSpecs.animation->frameRectWidthHeight.y;
                this->_sprite.setTextureRect(rect);
            }

    }  // namespace Rengine
}  // namespace Graphics
