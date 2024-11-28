#pragma once

#include <utility>

class Velocity {
    public:
        Velocity(float x, float y);
        ~Velocity();
        void reset();
        std::pair<float, float> getBase();
        float x;
        float y;
    private:
        float originalX;
        float originalY;
};
