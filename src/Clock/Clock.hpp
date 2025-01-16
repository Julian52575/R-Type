//
#ifndef SRC_CLOCK_HPP_
#define SRC_CLOCK_HPP_

#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <iostream>

namespace Rengine {
    namespace Clock {

        /**
        * @fn restart
        * @brief Restart the clock to 0.
        */
        void restart(void) noexcept;

        /**
        * @fn getElapsedTime
        * @template I An integer or floating point number
        * @brief Returns the elapsed time since the last restart / program start if restart was never called.
        */
        template <class I>
        I getElapsedTime(void) noexcept;

    }  // namespace Clock
}  // namespace Rengine

// Compile once only for lib
#ifdef __RENGINE_COMPILING_LIB
    // Yes, include tpp for templated function + unaccesible static variable
    #include "Clock.tpp"
#endif
#endif  // SRC_CLOCK_HPP_
