//
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
        static inline const std::chrono::time_point<std::chrono::steady_clock> __start = std::chrono::steady_clock::now();

        void restart(void) noexcept
        {
            // Using pointer to remove constness, __start was marked const to avoid user messing with it.
            std::chrono::time_point<std::chrono::steady_clock>* ptrNoMoreConst = const_cast<std::chrono::time_point<std::chrono::steady_clock>*>(&__start);

            *ptrNoMoreConst = std::chrono::steady_clock::now();
        }

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
