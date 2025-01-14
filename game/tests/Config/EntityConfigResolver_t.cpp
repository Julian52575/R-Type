//
#include <gtest/gtest.h>

#include "../../src/Config/EntityConfigResolver.hpp"

TEST(EntityConfigResolver, get)
{
    RType::Config::EntityConfigResolver resolver;
    RType::Config::EntityConfig& conf = resolver.get("Config/entity.json");
    RType::Config::EntityConfig& conf2 = resolver.get("Config/entity.json");

    EXPECT_EQ(std::addressof(conf), std::addressof(conf2));
}
TEST(EntityConfigResolverSingletone, get)
{
    RType::Config::EntityConfigResolverSingletone singletone1;
    RType::Config::EntityConfigResolverSingletone singletone2;

    EXPECT_EQ(std::addressof(singletone1.get()), std::addressof(singletone2.get()));
}
