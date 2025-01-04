//
#ifndef _SRC_GRAPHICS_SPRITESPECS_HPP_
#define _SRC_GRAPHICS_SPRITESPECS_HPP_
#include <cstdint>
#include <string>
#include "Vector2.hpp"

namespace Rengine {
    namespace Graphics {
        /**
        * @addtogroup Rengine::Graphics
        * @namespace Graphics
        * @class SpriteSpecs
        * @brief A structure of the specs needed to create a sprite.
        * The 'animation' field can be configured to match the spritesheet.
        */
        struct SpriteSpecs {
            /**
            * @brief The path to the spritesheet to apply to the sprite.
            */
            std::string texturePath;
            /**
            * @brief The offset of the sprite's origin (starting from the top left).
            * Default: 0x, 0y.
            */
            Rengine::Graphics::vector2D<int16_t> originOffset = {0, 0};
            /**
            * @brief The scaling to apply to the texture.
            * Default: 1x, 1y.
            */
            Rengine::Graphics::vector2D<float> textureScale = {1, 1};

            /**
            * @addtogroup Rengine::Graphics
            * @namespace Graphics
            * @class SpriteAnimationSpecs
            * @brief A structure of the specs needed to animate a sprite.
            */
            struct SpriteAnimationSpecs {
                /**
                * @brief The position of the first sprite's frame.
                * Default: 0x, 0y.
                */
                Rengine::Graphics::vector2D<uint16_t> frameRectXY = {0, 0};
                /**
                * @brief The size of each frame.
                * Default: 1000x, 1000y.
                */
                Rengine::Graphics::vector2D<uint16_t> frameRectWidthHeight = {1000, 1000};
                /**
                * @brief The number of frame in the spritesheet.
                * Default: 0
                */
                uint16_t frameCount = 0;
                /**
                * @brief The duration in seconds of each sprite.
                * Default: 0.8 (seconds).
                */
                float frameDuration = 0.8;
                /**
                * @brief The displacement between each frame in the spritesheet.
                * Default: 0x, 0y
                */
                Rengine::Graphics::vector2D<int16_t> frameDisplacement = {0, 0};
            };
            SpriteAnimationSpecs animation;
        };  // struct SpriteSpecs
    }  // namespace Graphics
}  //namespace Rengine
#endif
