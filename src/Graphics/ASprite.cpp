//
#include "ASprite.hpp"
#include "SpriteSpecs.hpp"
#include "Vector2.hpp"

namespace Rengine {
    namespace Graphics {

        ASprite::ASprite(const Rengine::Graphics::SpriteSpecs& spriteSpecs)
        {
            this->_spriteSpecs = spriteSpecs;
        }
        void ASprite::advanceFrame(const int16_t frameCount)
        {
            this->_currentFrame += frameCount;
            this->_currentFrame %= this->_spriteSpecs.animation.frameCount;
        }
        const Rengine::Graphics::SpriteSpecs& ASprite::getSpriteSpecs(void) const noexcept
        {
            return this->_spriteSpecs;
        }

    }  // namespace Graphics
}  // namespace Rengine
