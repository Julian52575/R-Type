//
#ifndef _SRC_GRAPHICS_SFML_SFMLSPRITE_HPP_
#define _SRC_GRAPHICS_SFML_SFMLSPRITE_HPP_
#include <SFML/Graphics.hpp>
#include <optional>

#include "../ASprite.hpp"
#include "../SpriteSpecs.hpp"

namespace Rengine {
    namespace Graphics {

        class SFMLSprite : public ASprite {
            public:
                SFMLSprite(const SpriteSpecs& spriteSpecs);
                ~SFMLSprite(void) = default;
                sf::Sprite& getSfSprite(void) noexcept;
                const sf::Sprite& getSfSprite(void) const noexcept;

            private:
                SpriteSpecs _spriteSpecs;
                sf::Sprite _sprite;
        };  // class SFMLSprite
    }  // namespace Rengine
}  // namespace Graphics

#endif
