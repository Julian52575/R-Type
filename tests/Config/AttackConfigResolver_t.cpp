//
#include <gtest/gtest.h>
#include <memory>

#include "../../src/Config/AttackConfigResolver.hpp"

TEST(AttackConfigResolver, get)
{
    RType::Config::AttackConfigResolver resolver;
    RType::Config::AttackConfig& conf = resolver.get("Config/missiles.json");
    RType::Config::AttackConfig& conf2 = resolver.get("Config/missiles.json");

    EXPECT_EQ(std::addressof(conf), std::addressof(conf2));
}
TEST(AttackConfigResolverSingletone, get)
{
    RType::Config::AttackConfigResolverSingletone singletone1;
    RType::Config::AttackConfigResolverSingletone singletone2;

    EXPECT_EQ(std::addressof(singletone1.get()), std::addressof(singletone2.get()));
}
