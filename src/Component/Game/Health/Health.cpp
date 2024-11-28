#include "Health.hpp"

Health::Health(int _health, int _maxHealth) {
    if (_health > _maxHealth)
        _health = _maxHealth;
    this->health = _health;
    this->maxHealth = _maxHealth;
}

Health::Health(int health) {
    this->health = health;
    this->maxHealth = health;
}

Health::~Health() {}

void Health::takeDamage(int damage) {
    this->health -= damage;
}