//
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <exception>
#include <string>

#include "SFMLText.hpp"
#include "../TextSpecs.hpp"

namespace Rengine {
    namespace Graphics {

        SFMLText::SFMLText(const TextSpecs& specs) : AText(specs)
        {
            if (this->_font.loadFromFile(specs.fontPath) == false) {
                throw TextException("Cannot load font '" + specs.fontPath + "'.");
            }
            this->_sfText.setFont(this->_font);
            this->_text = specs.message;
            this->_sfText.setString(specs.message);
            this->_sfText.setCharacterSize(specs.characterSize);
            this->_sfText.setOutlineThickness(specs.outlineThickness);
            this->applyStyle(specs.style);
            this->_sfText.setLetterSpacing(specs.letterSpacing);
            if (specs.lineSpacing != 0) {
                this->_sfText.setLineSpacing(specs.lineSpacing);
            }
            this->_sfText.setFillColor(
                    {specs.color.x,
                    specs.color.y,
                    specs.color.z}
            );
            this->_sfText.setOutlineColor(
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
                this->_sfText.setStyle(sf::Text::Style::Bold);
            }
            if (style & TextStyle::TextStyleItalic) {
                this->_sfText.setStyle(sf::Text::Style::Italic);
            }
            if (style & TextStyle::TextStyleStrikeThrough) {
                this->_sfText.setStyle(sf::Text::Style::StrikeThrough);
            }
            if (style & TextStyle::TextStyleUnderline) {
                this->_sfText.setStyle(sf::Text::Style::Underlined);
            }
        }

        sf::Text& SFMLText::getSfText(void) noexcept
        {
            return this->_sfText;
        }
        void SFMLText::setText(const std::string& text)
        {
            this->_sfText.setString(text);
            this->_text = text;
        }
        const std::string& SFMLText::getText(void) const noexcept
        {
            return this->_text;
        }


    }  // namespace Graphics
}  // namespace Rengine
