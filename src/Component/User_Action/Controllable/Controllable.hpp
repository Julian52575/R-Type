#pragma once
#include <SFML/Graphics.hpp>

class Controllable {
  public:
    Controllable();
    ~Controllable();

    sf::Keyboard::Key up = sf::Keyboard::Up;
    sf::Keyboard::Key down = sf::Keyboard::Down;
    sf::Keyboard::Key left = sf::Keyboard::Left;
    sf::Keyboard::Key right = sf::Keyboard::Right;  
};