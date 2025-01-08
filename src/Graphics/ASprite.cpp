//
#include <cstdint>

#include "ASprite.hpp"
#include "SpriteSpecs.hpp"
#include "Vector.hpp"

namespace Rengine {
    namespace Graphics {

        ASprite::ASprite(const Rengine::Graphics::SpriteSpecs& spriteSpecs, uint64_t creationTimeMicroseconds)
        {
            this->_spriteSpecs = spriteSpecs;
            this->_creationTimeMicroseconds = creationTimeMicroseconds;
        }
        const Rengine::Graphics::SpriteSpecs& ASprite::getSpriteSpecs(void) const noexcept
        {
            return this->_spriteSpecs;
        }
        bool ASprite::hasAnimation(void) const noexcept
        {
            return this->_spriteSpecs.animation.has_value();
        }

    }  // namespace Graphics
}  // namespace Rengine
