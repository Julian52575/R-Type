#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>


namespace bsf{
    class Timer {
        public:
            Timer(float timeLimit);
            ~Timer();
            void doAction(float deltaTime, std::function<void()> action);
        
        private:
            float currentTime;
            float timeLimit;
    };
}