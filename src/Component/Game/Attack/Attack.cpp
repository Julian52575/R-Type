#include "Attack.hpp"

Attack::Attack(int damage, float attackSpeed) {
    this->damage = damage;
    this->attackSpeed = attackSpeed;
    this->currentCooldown = 0.0f;
}

Attack::~Attack() {}

void Attack::update(float deltaTime) {
    this->currentCooldown += deltaTime;
}

bool Attack::canAttack() {
    return this->currentCooldown >= this->attackSpeed;
}

void Attack::reset() {
    this->currentCooldown = 0.0f;
}

int Attack::dealDamage() {
    return this->damage;
}