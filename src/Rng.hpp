//
#ifndef SRC_RNG_HPP_
#define SRC_RNG_HPP_

#include <cstdint>
#include <cstdlib>
#include <ctime>

namespace Rengine {


    #define RENGINE_RNG_RAND_MULTIPLIER 1664525
    #define RENGINE_RNG_RAND_INCREMENT 1013904223
    /*
    * @fn rngFunction
    * @return uint64_t A pseudo-randomly generated number
    * @brief Generate a random number from 0 to UINT64_MAX
    */
    static uint64_t rngFunction(void)
    {
        // straight from oot: https://github.com/zeldaret/oot/blob/main/src/libc64/qrand.c
        static uint64_t sRandInt = 0;

        if (sRandInt == 0) {
            std::srand(std::time(0));
            sRandInt = std::rand();
        }
        sRandInt = sRandInt * RENGINE_RNG_RAND_MULTIPLIER + RENGINE_RNG_RAND_INCREMENT;
        return sRandInt;
    }

}  // namespace Rengine

#endif  // SRC_RNG_HPP_
