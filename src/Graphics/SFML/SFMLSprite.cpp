//
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <exception>

#include "SFMLSprite.hpp"
#include "../ASprite.hpp"
#include "../SpriteSpecs.hpp"

namespace Rengine {
    namespace Graphics {

            SFMLSprite::SFMLSprite(const SpriteSpecs& spriteSpecs, uint64_t creationTickMicroseconds)
                : ASprite(spriteSpecs, creationTickMicroseconds), _renderObject(spriteSpecs)
            {
                // Load texture if set
                if (this->_spriteSpecs.texturePath != "") {
                    try {
                        this->_texture.loadFromFile(this->_spriteSpecs.texturePath);
                        this->_renderObject.setTexture(this->_spriteSpecs.type, this->_texture);
                    } catch (std::exception& e) {
                        std::string msg = e.what();

                        throw SpriteException(msg);
                    }
                }
                this->applySpecs(spriteSpecs);
            }
            void SFMLSprite::applySpecs(const SpriteSpecs& newSpecs)
            {
                // Color
                this->_renderObject.setColor(this->_spriteSpecs.type,
                        {this->_spriteSpecs.color.x, this->_spriteSpecs.color.y, this->_spriteSpecs.color.x,
                        this->_spriteSpecs.opacity} );
                // scale
                this->_renderObject.setScale(this->_spriteSpecs.type,
                        {this->_spriteSpecs.textureScale.x, this->_spriteSpecs.textureScale.y});
                // animation
                if (this->_spriteSpecs.animation.has_value() == true) {
                    sf::IntRect rect;

                    rect.left = this->_spriteSpecs.animation->frameRectXY.x;
                    rect.top = this->_spriteSpecs.animation->frameRectXY.y;
                    rect.width = this->_spriteSpecs.animation->frameRectWidthHeight.x;
                    rect.height = this->_spriteSpecs.animation->frameRectWidthHeight.y;
                    this->_renderObject.setTextureRect(this->_spriteSpecs.type, rect);
                }
                // origin
                this->_renderObject.setOrigin(this->_spriteSpecs.type,
                    {(float) this->_spriteSpecs.originOffset.x,
                    (float) this->_spriteSpecs.originOffset.y}
                );
                this->_spriteSpecs = newSpecs;
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
            void SFMLSprite::setPosition(const sf::Vector2f& pos)
            {
                this->_renderObject.setPosition(this->_spriteSpecs.type, pos);
            }

            sf::Sprite* SFMLSprite::getSfSprite(void) noexcept
            {
                return this->_renderObject.getSprite();
            }
            sf::CircleShape* SFMLSprite::getCircle(void) noexcept
            {
                return this->_renderObject.getCircle();
            }
            sf::RectangleShape* SFMLSprite::getRectangle(void) noexcept
            {
                return this->_renderObject.getRectangle();
            }
            void SFMLSprite::rotate(float rotation) noexcept
            {
                this->_renderObject.rotate(this->_spriteSpecs.type, rotation);
            }
            void SFMLSprite::flip(void) noexcept
            {
                sf::Vector2f vec = {this->_spriteSpecs.textureScale.x, this->_spriteSpecs.textureScale.y};

                if (this->_flipped == false) {
                    vec.x = this->_spriteSpecs.textureScale.x * -1;
                }
                this->_renderObject.setScale(this->_spriteSpecs.type, vec);
                this->_flipped = !this->_flipped;
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
                this->_renderObject.setTextureRect(this->_spriteSpecs.type, rect);
            }

            void SFMLSprite::updateSpriteSpecs(const SpriteSpecs& newSpecs)
            {
                // Load texture if set
                if (newSpecs.texturePath != "" && this->_spriteSpecs.texturePath != newSpecs.texturePath) {
                    try {
                        this->_texture.loadFromFile(this->_spriteSpecs.texturePath);
                        this->_renderObject.setTexture(this->_spriteSpecs.type, this->_texture);
                    } catch (std::exception& e) {
                        std::string msg = e.what();

                        throw SpriteException(msg);
                    }
                }
                this->_renderObject.updateSpriteSpecs(newSpecs);
                this->applySpecs(newSpecs);
            }
            void SFMLSprite::SFMLSpriteUnion::updateSpriteSpecs(const SpriteSpecs& newSpecs)
            {
                switch (newSpecs.type) {
                    case (SpriteType::SpriteTypeSprite):
                        break;

                    case (SpriteType::SpriteTypeCircle):
                        this->circle->setRadius(newSpecs.shapeData.specifics.circleRadius);
                        this->circle->setOutlineColor(
                                {newSpecs.shapeData.outlineColor.x,
                                newSpecs.shapeData.outlineColor.y,
                                newSpecs.shapeData.outlineColor.z}
                        );
                        this->circle->setOutlineThickness(newSpecs.shapeData.outlineThickness);
                        break;

                    case (SpriteType::SpriteTypeRectangle):
                        this->rectangle->setSize(
                                {newSpecs.shapeData.specifics.rectangleSize.x,
                                newSpecs.shapeData.specifics.rectangleSize.y}
                        );
                        this->rectangle->setOutlineColor(
                                {newSpecs.shapeData.outlineColor.x,
                                newSpecs.shapeData.outlineColor.y,
                                newSpecs.shapeData.outlineColor.z}
                        );
                        this->rectangle->setOutlineThickness(newSpecs.shapeData.outlineThickness);
                        break;

                    default:
                        throw SFMLSprite::SFMLSpriteUnion::SpriteTypeException();
                }
            }

            SFMLSprite::SFMLSpriteUnion::SFMLSpriteUnion(const SpriteSpecs& specs)
            {
                switch (specs.type) {
                    case (SpriteType::SpriteTypeSprite):
                        this->sprite = new sf::Sprite();
                        break;

                    case (SpriteType::SpriteTypeCircle):
                        this->circle = new sf::CircleShape();
                        this->circle->setRadius(specs.shapeData.specifics.circleRadius);
                        this->circle->setOutlineColor(
                                {specs.shapeData.outlineColor.x,
                                specs.shapeData.outlineColor.y,
                                specs.shapeData.outlineColor.z}
                        );
                        this->circle->setOutlineThickness(specs.shapeData.outlineThickness);
                        break;

                    case (SpriteType::SpriteTypeRectangle):
                        this->rectangle = new sf::RectangleShape();
                        this->rectangle->setSize(
                                {specs.shapeData.specifics.rectangleSize.x,
                                specs.shapeData.specifics.rectangleSize.y}
                        );
                        this->rectangle->setOutlineColor(
                                {specs.shapeData.outlineColor.x,
                                specs.shapeData.outlineColor.y,
                                specs.shapeData.outlineColor.z}
                        );
                        this->rectangle->setOutlineThickness(specs.shapeData.outlineThickness);
                        break;

                    default:
                        throw SFMLSprite::SFMLSpriteUnion::SpriteTypeException();
                }
            }
            SFMLSprite::SFMLSpriteUnion::~SFMLSpriteUnion()
            {
                // Hoping this works for all shape
                delete this->sprite;
            }
            void SFMLSprite::SFMLSpriteUnion::setTexture(SpriteType type, const sf::Texture& texture)
            {
                switch (type) {
                    case (SpriteType::SpriteTypeSprite):
                        this->sprite->setTexture(texture);
                        break;

                    case (SpriteType::SpriteTypeCircle):
                        this->circle->setTexture(&texture);
                        break;

                    case (SpriteType::SpriteTypeRectangle):
                        this->rectangle->setTexture(&texture);
                        break;

                    default:
                        break;
                }
            }
            void SFMLSprite::SFMLSpriteUnion::setScale(SpriteType type, const sf::Vector2f& scale)
            {
                switch (type) {
                    case (SpriteType::SpriteTypeSprite):
                        this->sprite->setScale(scale);
                        break;

                    case (SpriteType::SpriteTypeCircle):
                        this->circle->setScale(scale);
                        break;

                    case (SpriteType::SpriteTypeRectangle):
                        this->rectangle->setScale(scale);
                        break;

                    default:
                        break;
                }
            }
            sf::Vector2f SFMLSprite::SFMLSpriteUnion::getScale(SpriteType type) noexcept
            {
                switch (type) {
                    case (SpriteType::SpriteTypeSprite):
                        this->sprite->getScale();
                        break;

                    case (SpriteType::SpriteTypeCircle):
                        this->circle->getScale();
                        break;

                    case (SpriteType::SpriteTypeRectangle):
                        this->rectangle->getScale();
                        break;

                    default:
                        break;
                }
                return {1, 1};
            }
            void SFMLSprite::SFMLSpriteUnion::setTextureRect(SpriteType type, const sf::IntRect& rect)
            {
                switch (type) {
                    case (SpriteType::SpriteTypeSprite):
                        this->sprite->setTextureRect(rect);
                        break;

                    case (SpriteType::SpriteTypeCircle):
                        this->circle->setTextureRect(rect);
                        break;

                    case (SpriteType::SpriteTypeRectangle):
                        this->rectangle->setTextureRect(rect);
                        break;

                    default:
                        break;
                }
            }
            void SFMLSprite::SFMLSpriteUnion::setColor(SpriteType type, const sf::Color& color)
            {
                switch (type) {
                    case (SpriteType::SpriteTypeSprite):
                        this->sprite->setColor(color);
                        break;

                    case (SpriteType::SpriteTypeCircle):
                        this->circle->setFillColor(color);
                        break;

                    case (SpriteType::SpriteTypeRectangle):
                        this->rectangle->setFillColor(color);
                        break;

                    default:
                        break;
                }
            }
            void SFMLSprite::SFMLSpriteUnion::setPosition(SpriteType type, const sf::Vector2f& pos)
            {
                switch (type) {
                    case (SpriteType::SpriteTypeSprite):
                        this->sprite->setPosition(pos);
                        break;

                    case (SpriteType::SpriteTypeCircle):
                        this->circle->setPosition(pos);
                        break;

                    case (SpriteType::SpriteTypeRectangle):
                        this->rectangle->setPosition(pos);
                        break;

                    default:
                        break;
                }
            }
            void SFMLSprite::SFMLSpriteUnion::setOrigin(SpriteType type, const sf::Vector2f& origin)
            {
                switch (type) {
                    case (SpriteType::SpriteTypeSprite):
                        this->sprite->setOrigin(origin);
                        break;

                    case (SpriteType::SpriteTypeCircle):
                        this->circle->setOrigin(origin);
                        break;

                    case (SpriteType::SpriteTypeRectangle):
                        this->rectangle->setOrigin(origin);
                        break;

                    default:
                        break;
                }
            }
            void SFMLSprite::SFMLSpriteUnion::rotate(SpriteType type, float rotation) noexcept
            {
                switch (type) {
                    case (SpriteType::SpriteTypeSprite):
                        this->sprite->rotate(rotation);
                        break;

                    case (SpriteType::SpriteTypeCircle):
                        this->circle->setRotation(rotation);
                        break;

                    case (SpriteType::SpriteTypeRectangle):
                        this->rectangle->setRotation(rotation);
                        break;

                    default:
                        break;
                }
            }

            sf::Sprite* SFMLSprite::SFMLSpriteUnion::getSprite(void)
            {
                return this->sprite;
            }
            const sf::Sprite* SFMLSprite::SFMLSpriteUnion::getSprite(void) const
            {
                return this->sprite;
            }
            sf::CircleShape* SFMLSprite::SFMLSpriteUnion::getCircle(void)
            {
                return this->circle;
            }
            const sf::CircleShape* SFMLSprite::SFMLSpriteUnion::getCircle(void) const
            {
                return this->circle;
            }
            sf::RectangleShape* SFMLSprite::SFMLSpriteUnion::getRectangle(void)
            {
                return this->rectangle;
            }
            const sf::RectangleShape* SFMLSprite::SFMLSpriteUnion::getRectangle(void) const
            {
                return this->rectangle;
            }

    }  // namespace Rengine
}  // namespace Graphics
