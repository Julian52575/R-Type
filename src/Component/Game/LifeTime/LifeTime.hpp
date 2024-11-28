#pragma once

class LifeTime{
    public:
        LifeTime(float time);
        ~LifeTime();
        void update(float deltaTime);
        bool isDead();


    private:
        float time;
        float current_time;
};