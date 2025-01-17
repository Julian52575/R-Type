//
#include <gtest/gtest.h>
#include <lua5.4/lua.h>
#include <vector>

#include "../../src/Game/LuaManager.hpp"
TEST(LuaManager, loadScript)
{
    RType::LuaManager manager;
    int id = manager.loadLuaScript("Game/int1.lua");
    std::vector<RType::LuaReturn> antoine = manager.callFunction<float, float>("Game/int1.lua", id, "run", 1.0f, 2.0f);

    EXPECT_EQ(antoine.size(), 1);
    EXPECT_EQ(antoine[0].type, RType::LuaTypeInt);
    EXPECT_EQ(antoine[0].data.integer, 1);
}

TEST(LuaManager, callFunction)
{
    RType::LuaManager manager;

    int id = manager.loadLuaScript("Game/array.lua");   // return {{0, 1}, {10, 11}, {20, 21}}
    std::vector<RType::LuaReturn> antoine = manager.callFunction<>("Game/array.lua", id, "run");

    ASSERT_EQ(antoine.size(), 6);
    for (auto i = 0; i < antoine.size(); i++) {
        ASSERT_EQ(antoine[i].type, RType::LuaType::LuaTypeInt);
    }
    ASSERT_EQ(antoine[0].data.integer, 0);
    ASSERT_EQ(antoine[1].data.integer, 1);
    ASSERT_EQ(antoine[2].data.integer, 10);
    ASSERT_EQ(antoine[3].data.integer, 11);
    ASSERT_EQ(antoine[4].data.integer, 20);
    ASSERT_EQ(antoine[5].data.integer, 21);
}
