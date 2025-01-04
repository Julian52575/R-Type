//
#ifndef _SRC_GRAPHICS_ASPRITE_HPP_
#define _SRC_GRAPHICS_ASPRITE_HPP_

#include "SpriteSpecs.hpp"
#include "Vector2.hpp"

namespace Rengine {
    namespace Graphics {
        /**
        * @addtogroup Rengine::Graphics
        * @namespace Graphics
        * @class ASprite
        * @brief The sprite class, library independent.
        */
        class ASprite {
            public:
                ASprite(const Rengine::Graphics::SpriteSpecs& spriteSpecs);
                virtual ~ASprite(void) = default;
                void advanceFrame(const int16_t frameCount);
                const Rengine::Graphics::SpriteSpecs& getSpriteSpecs(void) const noexcept;

            private:
                Rengine::Graphics::SpriteSpecs _spriteSpecs;
                uint16_t _currentFrame = 0;
        };  // class IWindow
    }  // namespace Graphics
}  // namespace Rengine
#endif
