#pragma once
#include <SFML/Graphics.hpp>

class Hitbox {
    public:
        explicit Hitbox(sf::Vector2f size, sf::Vector2f offset = sf::Vector2f(0.0f, 0.0f));
        ~Hitbox();
    private:
        sf::Vector2f size;
        sf::Vector2f offset;
        bool isBlocking;
};

