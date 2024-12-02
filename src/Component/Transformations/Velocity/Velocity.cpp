#include "src/Component/Transformations/Velocity/Velocity.hpp"
#include <utility>

Velocity::Velocity(float x, float y){
    this->x = x;
    this->y = y;
    this->originalX = x;
    this->originalY = y;
}

Velocity::~Velocity() {}

void Velocity::reset() {
    this->x = this->originalX;
    this->y = this->originalY;
}

std::pair<float, float> Velocity::getBase() {
    return std::make_pair(this->originalX, this->originalY);
}

