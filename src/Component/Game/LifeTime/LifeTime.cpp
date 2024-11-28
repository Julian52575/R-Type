#include "LifeTime.hpp"

LifeTime::LifeTime(float time) : time(time) {
    this->current_time = 0.0f;
}

LifeTime::~LifeTime() {}

void LifeTime::update(float deltaTime) {
    this->current_time += deltaTime;
}

bool LifeTime::isDead() {
    return this->current_time >= this->time;
}
