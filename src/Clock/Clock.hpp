//
#ifndef SRC_CLOCK_CLOCK_HPP_
#define SRC_CLOCK_CLOCK_HPP_

#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iostream>

namespace Rengine {
    namespace Clock {

        /**
        * @fn restart
        * @brief restart the clock to 0.
        */
        void restart(void) noexcept;

        /**
        * @fn getElapsedTimeSeconds
        * @brief Returns the elapsed time in seconds since the last restart / program start if restart was never called.
        */
        double getElapsedTime(void) noexcept;

    }  // namespace Clock
}  // namespace Rengine
#endif  // SRC_CLOCK_CLOCK_HPP_
