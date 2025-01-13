//
#include <gtest/gtest.h>

#include "../../src/Config/LevelConfigResolver.hpp"

TEST(LevelConfigResolver, get)
{
    RType::Config::LevelConfigResolver resolver;
    RType::Config::LevelConfig& conf = resolver.get("Config/level.json");
    RType::Config::LevelConfig& conf2 = resolver.get("Config/level.json");

    EXPECT_EQ(std::addressof(conf), std::addressof(conf2));
}
TEST(LevelConfigResolverSingletone, get)
{
    RType::Config::LevelConfigResolverSingletone singletone1;
    RType::Config::LevelConfigResolverSingletone singletone2;

    EXPECT_EQ(std::addressof(singletone1.get()), std::addressof(singletone2.get()));
}
