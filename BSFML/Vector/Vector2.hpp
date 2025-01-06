#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>


namespace bsf {
    class Vector2 : public sf::Vector2f {
        public:
            Vector2();
            Vector2(float x, float y);
            Vector2(const sf::Vector2f& other);
            Vector2(const sf::Vector2i& other);
            Vector2(const sf::Vector2u& other);
            
            ~Vector2();
            float getLength();
            Vector2 normalize();
            float distanceTo(const Vector2& other);
            Vector2 operator+(const Vector2& other);
            Vector2 operator-(const Vector2& other);
            Vector2 operator*(const float& scalar);
            Vector2 operator/(const float& scalar);
            Vector2 operator+=(const Vector2& other);
            Vector2 operator-=(const Vector2& other);
            Vector2 operator*=(const float& scalar);
            Vector2 operator/=(const float& scalar);
            bool operator==(const Vector2& other);
            bool operator!=(const Vector2& other);

            Vector2 &operator=(const sf::Vector2f& other);
            Vector2 &operator=(const sf::Vector2i& other);
            Vector2 &operator=(const sf::Vector2u& other);
    };
};