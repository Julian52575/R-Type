//
#ifndef _SRC_GRAPHICS_SFML_SFMLSPRITE_HPP_
#define _SRC_GRAPHICS_SFML_SFMLSPRITE_HPP_
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <optional>

#include "../ASprite.hpp"
#include "../SpriteSpecs.hpp"

namespace Rengine {
    namespace Graphics {

        class SFMLSprite : public ASprite {
            public:
                SFMLSprite(const SpriteSpecs& spriteSpecs);
                ~SFMLSprite(void) = default;
                void advanceFrame(const int16_t frameCount);
                sf::Sprite& getSfSprite(void) noexcept;
                const sf::Sprite& getSfSprite(void) const noexcept;
                void setSpriteSpecs(const Rengine::Graphics::SpriteSpecs& spriteSpecs);

            private:
                sf::Sprite _sprite;
                sf::Texture _texture;
        };  // class SFMLSprite
    }  // namespace Rengine
}  // namespace Graphics

#endif
