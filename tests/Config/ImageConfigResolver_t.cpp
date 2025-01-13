//
#include <gtest/gtest.h>

#include "../../src/Config/ImageConfigResolver.hpp"

TEST(ImageConfigResolver, get)
{
    RType::Config::ImageConfigResolver resolver;
    RType::Config::ImageConfig& conf = resolver.get("Config/image.json");
    RType::Config::ImageConfig& conf2 = resolver.get("Config/image.json");

    EXPECT_EQ(std::addressof(conf), std::addressof(conf2));
}
TEST(ImageConfigResolverSingletone, get)
{
    RType::Config::ImageConfigResolverSingletone singletone1;
    RType::Config::ImageConfigResolverSingletone singletone2;

    EXPECT_EQ(std::addressof(singletone1.get()), std::addressof(singletone2.get()));
}
