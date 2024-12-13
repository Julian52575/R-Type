#pragma once
#include <SFML/Graphics.hpp>

class Camera : sf::View {
    public:
        Camera(sf::FloatRect rect);
        ~Camera();
        void update(float x, float y);
        sf::View getView();
    private:
        float x;
        float y;
};