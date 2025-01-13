//
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <exception>

#include "SFMLText.hpp"
#include "../TextSpecs.hpp"

namespace Rengine {
    namespace Graphics {

        SFMLText::SFMLText(const TextSpecs& specs) : AText(specs)
        {
            if (this->_font.loadFromFile(specs.fontPath) == false) {
                throw TextException("Cannot load font '" + specs.fontPath + "'.");
            }
            this->_text.setFont(this->_font);
            this->_text.setString(specs.message);
            this->_text.setCharacterSize(specs.characterSize);
            this->_text.setOutlineThickness(specs.outlineThickness);
            this->applyStyle(specs.style);
            this->_text.setLetterSpacing(specs.letterSpacing);
            if (specs.lineSpacing != 0) {
                this->_text.setLineSpacing(specs.lineSpacing);
            }
            this->_text.setFillColor(
                    {specs.color.x,
                    specs.color.y,
                    specs.color.z}
            );
            this->_text.setOutlineColor(
                    {specs.outlineColor.x,
                    specs.outlineColor.y,
                    specs.outlineColor.z}
            );
        }

        void SFMLText::applyStyle(uint8_t style)
        {
            if (style == TextStyle::TextStyleRegular) {
                return;
            }
            if (style & TextStyle::TextStyleBold) {
                this->_text.setStyle(sf::Text::Style::Bold);
            }
            if (style & TextStyle::TextStyleItalic) {
                this->_text.setStyle(sf::Text::Style::Italic);
            }
            if (style & TextStyle::TextStyleStrikeThrough) {
                this->_text.setStyle(sf::Text::Style::StrikeThrough);
            }
            if (style & TextStyle::TextStyleUnderline) {
                this->_text.setStyle(sf::Text::Style::Underlined);
            }
        }

        sf::Text& SFMLText::getText(void) noexcept
        {
            return this->_text;
        }

    }  // namespace Rengine
}  // namespace Graphics
