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
        * @brief Don't interact with this variable yourself. Use restart or getElapsedTime instead.
        */
        const std::chrono::time_point<std::chrono::steady_clock> __start = std::chrono::steady_clock::now();

        /**
        * @fn restart
        * @brief restart the clock to 0.
        */
        inline void restart(void) noexcept
        {
            // Using pointer to remove constness, __start was marked const to avoid user messing with it.
            std::chrono::time_point<std::chrono::steady_clock>* ptrNoMoreConst = (std::chrono::time_point<std::chrono::steady_clock> *) (&__start);

            *ptrNoMoreConst = std::chrono::steady_clock::now();
        }

        /**
        * @fn getElapsedTime
        * @template I An integer or floating point number
        * @brief Returns the elapsed time since the last restart / program start if restart was never called.
        */
        template <class I>
        I getElapsedTime(void) noexcept
        {
            static_assert(std::is_integral<I>::value || std::is_floating_point_v<I>); // Ensure the templated class is either a integer or a floating number
            std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
            std::chrono::duration<I> elapsed = end - __start;

            return elapsed.count();
        }

    }  // namespace Clock
}  // namespace Rengine
#endif  // SRC_CLOCK_HPP_
