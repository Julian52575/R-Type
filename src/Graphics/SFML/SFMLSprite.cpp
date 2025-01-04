//
#include "SFMLSprite.hpp"
#include "../ASprite.hpp"
#include "../SpriteSpecs.hpp"

namespace Rengine {
    namespace Graphics {

            SFMLSprite::SFMLSprite(const SpriteSpecs& spriteSpecs) : ASprite(spriteSpecs)
            {
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
