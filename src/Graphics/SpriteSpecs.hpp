//
#ifndef SRC_GRAPHICS_SPRITESPECS_HPP_
#define SRC_GRAPHICS_SPRITESPECS_HPP_
#include <cstdint>
#include <string>
#include <optional>

#include "Vector.hpp"

namespace Rengine {
    namespace Graphics {

        /**
        * @brief The type of sprite supported by the Rengine.
        */
        enum SpriteType {
            SpriteTypeNA,
            SpriteTypeSprite,
            SpriteTypeRectangle,
            SpriteTypeCircle
        };
        /**
        * @addtogroup Rengine::Graphics
        * @namespace Graphics
        * @class SpriteSpecs
        * @brief A structure of the specs needed to create a sprite.
        * The 'animation' field can be configured to match the spritesheet.
        */
        struct SpriteSpecs {
            /**
            * @brief A union containing the data for all shapes.
            */
            union ShapeSpecifics {
                float circleRadius;
                Rengine::Graphics::vector2D<float> rectangleSize = {192, 108};
            };
            /**
            * @brief This structure manages the data for the Circle and Rectangle sprite.
            */
            struct ShapeData {
                /**
                * @brief The shape-specific data like circleRadius and rectangleSize.
                * Default: 1
                */
                ShapeSpecifics specifics;
                /**
                * @brief The size of the shape's outline.
                * Default: 1
                */
                float outlineThickness = 1;
                /**
                * @brief The shape's outline color.
                * Default: White {255, 255, 255}
                */
                Rengine::Graphics::vector3D<uint8_t> outlineColor = {255, 255, 255};
            };
            /**
            * @brief The type of sprite.
            * Default: simple sprite
            */
            SpriteType type = SpriteTypeSprite;
            /**
            * @brief The data for the Circle and Rectangle sprite. Ignored when constructing a simple sprite.
            */
            ShapeData shapeData;
            /**
            * @brief The path to the spritesheet to apply to the sprite. Leave empty for no texture.
            */
            std::string texturePath;
            /**
            * @brief The sprite's color.
            * x = Red
            * y = Blue
            * z = Green
            * Default: White (255, 255, 255)
            */
            Rengine::Graphics::vector3D<uint8_t> color = {255, 255, 255};
            /**
            * @brief The transparency value of the sprite.
            * 0 = fully transparent
            * Default: fully visible (255)
            */
            uint8_t opacity = 255;
            /**
            * @brief The offset of the sprite's origin (starting from the top left).
            * Default: 0x, 0y.
            */
            Rengine::Graphics::vector2D<float> originOffset = {0, 0};
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
                uint16_t frameCount = 1;
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
            std::optional<SpriteAnimationSpecs> animation;
        };  // struct SpriteSpecs
    }  // namespace Graphics
}  // namespace Rengine
#endif  // SRC_GRAPHICS_SPRITESPECS_HPP_
