#include "Timer.hpp"

bsf::Timer::Timer(float timeLimit) {
    currentTime = 0;
    this->timeLimit = timeLimit;
}

bsf::Timer::~Timer() {
}

void bsf::Timer::doAction(float deltaTime, std::function<void()> action) {
    currentTime += deltaTime;
    if (currentTime >= timeLimit) {
        currentTime = 0;
        action();
    }
}