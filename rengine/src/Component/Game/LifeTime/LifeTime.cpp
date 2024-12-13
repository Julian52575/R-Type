#include "./LifeTime.hpp"



Component::LifeTime::LifeTime(float time) : time(time) {
    this->current_time = 0.0f;
}

Component::LifeTime::~LifeTime() {}

void Component::LifeTime::update(float deltaTime) {
    this->current_time += deltaTime;
}

bool Component::LifeTime::isDead() {
    return this->current_time >= this->time;
}
