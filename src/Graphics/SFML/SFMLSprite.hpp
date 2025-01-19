//
#ifndef _SRC_GRAPHICS_SFML_SFMLSPRITE_HPP_
#define _SRC_GRAPHICS_SFML_SFMLSPRITE_HPP_
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Vector3.hpp>
#include <optional>

#include "../ASprite.hpp"
#include "../SpriteSpecs.hpp"

namespace Rengine {
    namespace Graphics {
        /**
        * @addtogroup Rengine::Graphics
        * @namespace Graphics
        * @class SFMLSprite
        * @brief Implementation of ASprite using the SFML library.
        * See ASprite.
        */
        class SFMLSprite : public ASprite {
            public:
                SFMLSprite(const SpriteSpecs& spriteSpecs, uint64_t creationTimeMicroseconds);
                ~SFMLSprite(void) = default;
                void advanceFrame(int16_t frameCount = 1);
                void advanceFrameFromTime(uint64_t currentTimeMicroseconds);
                void setPosition(const sf::Vector2f& pos);
                sf::Sprite* getSfSprite(void) noexcept;
                sf::CircleShape* getCircle(void) noexcept;
                sf::RectangleShape* getRectangle(void) noexcept;
                void rotate(float rotation) noexcept;
                void flip(void) noexcept;
                void updateSpriteSpecs(const SpriteSpecs& newSpecs);

            private:
                union SFMLSpriteUnion {
                    class SpriteTypeException : public std::exception {
                        public:
                            const char *what() const noexcept { return "Invalid Sprite Type in class SFMLSprite."; };
                    };
                    public:
                        /**
                         * @ brief The shape-specific data is applied here.
                        */
                        SFMLSpriteUnion(const SpriteSpecs& spriteSpecs);
                        ~SFMLSpriteUnion();
                        void setTexture(SpriteType type, const sf::Texture& texture);
                        void setScale(SpriteType type, const sf::Vector2f& scale);
                        sf::Vector2f getScale(SpriteType type) noexcept;
                        void setTextureRect(SpriteType type, const sf::IntRect& rect);
                        void setColor(SpriteType type, const sf::Color& color);
                        void setPosition(SpriteType type, const sf::Vector2f& pos);
                        void setOrigin(SpriteType type, const sf::Vector2f& origin);
                        void rotate(SpriteType type, float rotation) noexcept;
                        sf::Sprite* getSprite(void);
                        const sf::Sprite* getSprite(void) const;
                        sf::CircleShape* getCircle(void);
                        const sf::CircleShape* getCircle(void) const;
                        sf::RectangleShape* getRectangle(void);
                        const sf::RectangleShape* getRectangle(void) const;
                        void updateSpriteSpecs(const SpriteSpecs& specs);

                    private:
                        // Using pointer to make the union work
                        sf::Sprite* sprite;
                        sf::RectangleShape* rectangle;
                        sf::CircleShape* circle;
                };
                void applySpecs(const SpriteSpecs& specs);
                void applyCurrentFrameTexture(void);
                SFMLSpriteUnion _renderObject;
                sf::Texture _texture;
                bool _flipped = false;
        };  // class SFMLSprite
    }  // namespace Rengine
}  // namespace Graphics

#endif  // _SRC_GRAPHICS_SFML_SFMLSPRITE_HPP_
