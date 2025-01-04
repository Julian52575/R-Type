//
#include "ASprite.hpp"
#include "SpriteSpecs.hpp"
#include "Vector.hpp"

namespace Rengine {
    namespace Graphics {

        ASprite::ASprite(const Rengine::Graphics::SpriteSpecs& spriteSpecs)
        {
            this->_spriteSpecs = spriteSpecs;
        }
        const Rengine::Graphics::SpriteSpecs& ASprite::getSpriteSpecs(void) const noexcept
        {
            return this->_spriteSpecs;
        }

    }  // namespace Graphics
}  // namespace Rengine
