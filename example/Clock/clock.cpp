//
#include <iostream>
#include <rengine/Rengine.hpp>

int main(void)
{
    double total = 0.0f;

    while (total < 10.0f) {
        // for (int i = 0; i < INT32_MAX; i++);  // Uncomment to make loop slower

        double deltatime = Rengine::Clock::getElapsedTime<double>();
        //void *compilationError = Rengine::Clock::getElapsedTime<void *>();  // Uncomment for compilation error due to bad template type

        Rengine::Clock::restart();
        total += deltatime;
        std::cout << "Total = " << total << ". Deltatime = " << deltatime << std::endl;
    }
    return 0;
}
