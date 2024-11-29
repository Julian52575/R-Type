#include "Text.hpp"

Text::Text(const std::string& filename, const std::string& text, int size, sf::Color color, std::pair<float,float> scale,std::pair<float,float> origin) {
    if (!font.loadFromFile(filename))
        throw std::runtime_error("Could not load font");
    setFont(font);
    setString(text);
    setCharacterSize(size);
    setFillColor(color);
    setOrigin(origin.first,origin.second);
    setScale(scale.first,scale.second);
}

Text::~Text() {}