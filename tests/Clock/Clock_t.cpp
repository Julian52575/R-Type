//
#include <chrono>
#include <cstdint>
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>
#include <unistd.h>
#include <thread>

#include "../../src/Clock/Clock.hpp"
TEST(Clock, restart)
{
    Rengine::Clock::restart();

    // Expect deltatime to be low
    EXPECT_TRUE(Rengine::Clock::getElapsedTime<double>() < 1000);
}

TEST(Clock, getElapsedTime)
{
    double tab[8];

    for (int i = 0; i < 8; i++) {
        Rengine::Clock::restart();
        // sleep for half a second
        std::this_thread::sleep_for(std::chrono::microseconds(500000));
        tab[i] = Rengine::Clock::getElapsedTime<double>();
    }
    for (int i = 0; i < 8; i++) {
        // best I can do is check if deltatime is supertior to the sleep time
        EXPECT_GT(tab[i], 0.5f);
    }
}
