#include "./Hitbox.hpp"

Hitbox::Hitbox(sf::Vector2f size, sf::Vector2f offset) {
    this->size = size;
    this->offset = offset;
    this->isBlocking = false;
}

Hitbox::~Hitbox() {}

sf::Vector2f &Hitbox::getSize() {
    return this->size;
}

sf::Vector2f &Hitbox::getOffset() {
    return this->offset;
}

void Hitbox::setSize(sf::Vector2f size) {
    this->size = size;
}

void Hitbox::setOffset(sf::Vector2f offset) {
    this->offset = offset;
}

void Hitbox::setSize(float x, float y) {
    this->size = sf::Vector2f(x, y);
}

void Hitbox::setOffset(float x, float y) {
    this->offset = sf::Vector2f(x, y);
}