#include "Vector2.hpp"

bsf::Vector2::Vector2() : sf::Vector2f() {
}

bsf::Vector2::Vector2(float x, float y) : sf::Vector2f(x, y) {

}

bsf::Vector2::Vector2(const sf::Vector2f& other) : sf::Vector2f(other) {
}

bsf::Vector2::Vector2(const sf::Vector2i& other) : sf::Vector2f(other) {
}

bsf::Vector2::Vector2(const sf::Vector2u& other) : sf::Vector2f(other) {
}

bsf::Vector2::~Vector2() {
}

bsf::Vector2 bsf::Vector2::normalize() {
    float length = getLength();
    return bsf::Vector2(this->x / length, this->y / length);
}

float bsf::Vector2::getLength() {
    return sqrt(this->x * this->x + this->y * this->y);
}

float bsf::Vector2::distanceTo(const bsf::Vector2& other) {
    return sqrt(pow(this->x - other.x, 2) + pow(this->y - other.y, 2));
}

bsf::Vector2 bsf::Vector2::operator+(const bsf::Vector2& other) {
    return bsf::Vector2(this->x + other.x, this->y + other.y);
}

bsf::Vector2 bsf::Vector2::operator-(const bsf::Vector2& other) {
    return bsf::Vector2(this->x - other.x, this->y - other.y);
}

bsf::Vector2 bsf::Vector2::operator*(const float& scalar) {
    return bsf::Vector2(this->x * scalar, this->y * scalar);
}

bsf::Vector2 bsf::Vector2::operator/(const float& scalar) {
    return bsf::Vector2(this->x / scalar, this->y / scalar);
}

bsf::Vector2 bsf::Vector2::operator+=(const bsf::Vector2& other) {
    this->x += other.x;
    this->y += other.y;
    return *this;
}

bsf::Vector2 bsf::Vector2::operator-=(const bsf::Vector2& other) {
    this->x -= other.x;
    this->y -= other.y;
    return *this;
}

bsf::Vector2 bsf::Vector2::operator*=(const float& scalar) {
    this->x *= scalar;
    this->y *= scalar;
    return *this;
}

bsf::Vector2 bsf::Vector2::operator/=(const float& scalar) {
    this->x /= scalar;
    this->y /= scalar;
    return *this;
}

bool bsf::Vector2::operator==(const bsf::Vector2& other) {
    return this->x == other.x && this->y == other.y;
}

bool bsf::Vector2::operator!=(const bsf::Vector2& other) {
    return this->x != other.x || this->y != other.y;
}

bsf::Vector2 &bsf::Vector2::operator=(const sf::Vector2f& other) {
    this->x = other.x;
    this->y = other.y;
    return *this;
}

bsf::Vector2 &bsf::Vector2::operator=(const sf::Vector2i& other) {
    this->x = other.x;
    this->y = other.y;
    return *this;
}

bsf::Vector2 &bsf::Vector2::operator=(const sf::Vector2u& other) {
    this->x = other.x;
    this->y = other.y;
    return *this;
}