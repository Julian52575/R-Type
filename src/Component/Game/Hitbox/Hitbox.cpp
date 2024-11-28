#include "Hitbox.hpp"

Hitbox::Hitbox(sf::Vector2f size, sf::Vector2f offset) {
    this->size = size;
    this->offset = offset;
    this->isBlocking = false;
}

Hitbox::~Hitbox() {}
