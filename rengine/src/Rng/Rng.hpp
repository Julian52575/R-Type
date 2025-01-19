//
#ifndef SRC_RNG_HPP_
#define SRC_RNG_HPP_

#include <cstdint>
#include <cstdlib>
#include <ctime>

namespace Rengine {
        namespace RNG {

        /*
        * @fn rngFunction
        * @return uint64_t A pseudo-randomly generated number
        * @brief Generate a random number from 0 to UINT64_MAX
        */
        static uint64_t rngFunction(void) noexcept
        {
            // straight out of oot: https://github.com/zeldaret/oot/blob/main/src/libc64/qrand.c
            static uint64_t sRandInt = 0;

            if (sRandInt == 0) {
                std::srand(std::time(0));
                sRandInt = std::rand();
            }
            sRandInt = sRandInt * 1664525 + 1013904223;
            return sRandInt;
        }

    }  // namespace RNG
}  // namespace Rengine

#endif  // SRC_RNG_HPP_
