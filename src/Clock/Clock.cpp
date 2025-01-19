//
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iostream>

#include "Clock.hpp"
namespace Rengine {
    namespace Clock {

        /**
        * @brief Don't interact with this variable yourself. Use restart or getElapsedTime instead.
        */
        static std::chrono::time_point<std::chrono::steady_clock> __start = std::chrono::steady_clock::now();

        /**
        * @fn restart
        * @brief restart the clock to 0.
        */
        void restart(void) noexcept
        {
            // Using pointer to remove constness, __start was marked const to avoid user messing with it.
            __start = std::chrono::steady_clock::now();
        }

        double getElapsedTime(void) noexcept
        {
            std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
            std::chrono::duration<double> elapsed = end - __start;

            return elapsed.count();
        }

    }  // namespace Clock
}  // namespace Rengine
