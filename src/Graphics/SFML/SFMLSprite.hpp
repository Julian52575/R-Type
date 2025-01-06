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
                sf::Sprite& getSfSprite(void) noexcept;
                const sf::Sprite& getSfSprite(void) const noexcept;

            private:
                void applyCurrentFrameTexture(void);
                sf::Sprite _sprite;
                sf::Texture _texture;
        };  // class SFMLSprite
    }  // namespace Rengine
}  // namespace Graphics

#endif
