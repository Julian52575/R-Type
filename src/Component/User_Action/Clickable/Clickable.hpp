#pragma once
#include <SFML/Graphics.hpp>

class Clickable{
    public:
        Clickable();
        ~Clickable();
        void onClick();
        void onRightClick();
        sf::FloatRect getBounds() const;
};

