#include "Camera.hpp"

Camera::Camera(sf::FloatRect rect) : sf::View(rect) {
    this->x = rect.left;
    this->y = rect.top;
}

Camera::~Camera() {}

void Camera::update(float x, float y) {
    this->x = x;
    this->y = y;
    this->setCenter(this->x, this->y);
}

sf::View Camera::getView() {
    return *this;
}