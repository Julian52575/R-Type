#include "src/Component/Game/Hitbox/Hitbox.hpp"

Hitbox::Hitbox(sf::Vector2f size, sf::Vector2f offset) {
    this->size = size;
    this->offset = offset;
    this->isBlocking = false;
}

Hitbox::~Hitbox() {}

sf::Vector2f Hitbox::getSize() {
    return this->size;
}

sf::Vector2f Hitbox::getOffset() {
    return this->offset;
}