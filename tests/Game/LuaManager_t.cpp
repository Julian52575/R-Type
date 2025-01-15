//
#include <gtest/gtest.h>
#include <lua5.4/lua.h>

#include "../../src/Game/LuaManager.hpp"
#include "any"
TEST(LuaManager, loadScript)
{
    RType::LuaManager manager;

    manager.addScript("Game/int1.lua");
    std::vector<RType::LuaReturn> antoine = manager.callFunction<float, float>("Game/int1.lua", "run", 1.0f, 2.0f);

    EXPECT_EQ(antoine.size(), 1);
    EXPECT_EQ(antoine[0].type, RType::LuaTypeInt);
    EXPECT_EQ(antoine[0].data.integer, 1);
}
