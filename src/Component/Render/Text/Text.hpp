#pragma once
#include <SFML/Graphics.hpp>
#include <utility>
#include <string>

class Text : public sf::Text {
    public:
        Text(const std::string& filename, const std::string& text, int size, sf::Color color, std::pair<float, float> scale = std::make_pair(1.f, 1.f), std::pair<float, float> origin = std::make_pair(0.f, 0.f));
        ~Text();
    private:
        sf::Font font;
};

