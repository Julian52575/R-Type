#include <gtest/gtest.h>
#include <memory>
#include "../src/ECS.hpp"

TEST(Entity, addComponent)
{
    Rengine::ECS ecs;
    Rengine::Entity &e0 = ecs.addEntity();

    EXPECT_THROW(e0.addComponent<int>(0), Rengine::ComponentRegistryExceptionNotRegistred);
    Rengine::SparseArray<int>& sp = ecs.registerComponent<int>();

    int& con = e0.addComponent<int>(0);
    EXPECT_TRUE(sp[int(e0)].has_value());
    EXPECT_EQ(std::addressof(sp[int(e0)].value()), std::addressof(con));
}
