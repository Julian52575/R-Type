//
#include <gtest/gtest.h>

#include "../../src/Config/ConfigurationIdResolver.hpp"
TEST(ConfigurationIdResolver, getUint16)
{
    RType::Config::ConfigurationIdResolver con("Config/id.json");

    EXPECT_EQ(con.get(1), "number1");
    EXPECT_EQ(con.get(2), "number2");
    EXPECT_EQ(con.get(3), "number3");
    EXPECT_EQ(con.get(42), "number42");
    EXPECT_EQ(con.get(-1), "");
}
TEST(ConfigurationIdResolver, getString)
{
    RType::Config::ConfigurationIdResolver con("Config/id.json");

    EXPECT_EQ(con.get("number1"), 1);
    EXPECT_EQ(con.get("number2"), 2);
    EXPECT_EQ(con.get("number3"), 3);
    EXPECT_EQ(con.get("number42"), 42);
    EXPECT_EQ(con.get("number84"), (uint16_t) -1);
}
