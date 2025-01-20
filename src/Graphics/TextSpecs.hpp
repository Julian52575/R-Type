//
#ifndef SRC_GRAPHICS_TEXTSPECS_HPP_
#define SRC_GRAPHICS_TEXTSPECS_HPP_

#include "Vector.hpp"
#include <string>

namespace Rengine {
    namespace Graphics {

        /**
        * @addtogroup Rengine::Graphics
        * @namespace Graphics
        * @class TextSpecs
        * @brief An enum of the supported text style.
        * Note: TextStyleRegular will overwrite the other style.
        */
        enum TextStyle {
            TextStyleRegular = 0b0000,
            TextStyleItalic = 0b0001,
            TextStyleBold = 0b0010,
            TextStyleStrikeThrough = 0b0100,
            TextStyleUnderline = 0b1000
        };

        /**
        * @addtogroup Rengine::Graphics
        * @namespace Graphics
        * @class TextSpecs
        * @brief A structure of the specs needed to create a text.
        */
        struct TextSpecs {
            /**
            * @brief The message to be displayed.
            */
            std::string message = "Powered by rengine.";
            /**
            * @brief The font to use.
            * Default: "" (will likely create an exception when used to create the text)
            */
            std::string fontPath;
            /**
            * @brief The size of the characters.
            * Default: 64
            */
            unsigned int characterSize = 64;
            /**
            * @brief The color of the characters.
            * x = red
            * y = blue
            * z = green
            * Default: Black {0, 0, 0}
            */
            Rengine::Graphics::vector3D<uint8_t> color = {0, 0, 0};
            /**
            * @brief The size of the outline.
            */
            float outlineThickness = 2;
            /**
            * @brief The color of the outline.
            * Default: White {255, 255, 255}
            */
            Rengine::Graphics::vector3D<uint8_t> outlineColor = {255, 255, 255};
            /**
            * @brief Binary or Rengine::Graphics::TextStyle constants to create the style you want.
            * Default: TextStyle::TextStyleRegular
            */
            uint8_t style = TextStyleRegular;
            /**
            * @brief The spacing of the letters.
            * Default: 0
            */
            float letterSpacing = 0;
            /**
            * @brief The spacing of the lines.
            * Default: 0
            */
            float lineSpacing = 0;
        };

    }  // namespace Graphics
}  // namespace Rengine
#endif  // SRC_GRAPHICS_TEXTSPECS_HPP_
