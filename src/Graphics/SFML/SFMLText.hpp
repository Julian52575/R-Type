//
#ifndef SRC_GRAPHICS_SFML_SFMLTEXT_HPP_
#define SRC_GRAPHICS_SFML_SFMLTEXT_HPP_

#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <string>

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
                explicit SFMLText(const TextSpecs& specs);
                ~SFMLText() = default;
                sf::Text& getSfText(void) noexcept;
                void setText(const std::string& text);
                const std::string& getText(void) const noexcept;

            private:
                void applyStyle(uint8_t style);
                std::string _text;
                sf::Text _sfText;
                sf::Font _font;
        };
    }  // namespace Graphics
}  // namespace Rengine
#endif  // SRC_GRAPHICS_SFML_SFMLTEXT_HPP_
