#pragma once
#include <SFML/Graphics.hpp>

class Hitbox {
    public:
        explicit Hitbox(sf::Vector2f size, sf::Vector2f offset = sf::Vector2f(0.0f, 0.0f));
        ~Hitbox();
        sf::Vector2f &getSize();
        sf::Vector2f &getOffset();
        void setSize(sf::Vector2f size);
        void setOffset(sf::Vector2f offset);
        void setSize(float x, float y);
        void setOffset(float x, float y);
    private:
        sf::Vector2f size;
        sf::Vector2f offset;
        bool isBlocking;
};

