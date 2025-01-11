//
#ifndef SRC_GRAPHICS_SFML_SFMLTEXT_HPP_
#define SRC_GRAPHICS_SFML_SFMLTEXT_HPP_

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

#include "../AText.hpp"
#include "../TextSpecs.hpp"

namespace Rengine {
    namespace Graphics {

        /**
        * @addtogroup Rengine::Graphics
        * @namespace Graphics
        * @class SFMLText
        * @brief Implementation of AText using the SFML library.
        * See AText.
        */
        class SFMLText : public AText {
            public:
                SFMLText(const TextSpecs& specs);
                ~SFMLText() = default;
                sf::Text& getText(void) noexcept;

            private:
                void applyStyle(uint8_t style);
                sf::Text _text;
                sf::Font _font;
        };
    }  // namespace Rengine
}  // namespace Graphics
#endif  // SRC_GRAPHICS_SFML_SFMLTEXT_HPP_
