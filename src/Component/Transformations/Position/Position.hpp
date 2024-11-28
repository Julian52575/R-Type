#pragma once
#include <unordered_map>
#include <any>
#include <string>

class Position  {
    public:
        Position(float x, float y);
        ~Position();
        float x;
        float y;
};