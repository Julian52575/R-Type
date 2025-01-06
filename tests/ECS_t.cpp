#include <gtest/gtest.h>
#include <memory>
#include "../src/ECS.hpp"

TEST(ECS, startingSize)
{
    Rengine::ECS::size_type size = 12;
    Rengine::ECS ecs(size);

    Rengine::SparseArray<int>& sp = ecs.registerComponent<int>();
    EXPECT_EQ(sp.size(), size);
}
TEST(ECS, addGetEntity)
{
    Rengine::ECS ecs;
    Rengine::Entity& e = ecs.addEntity();

    EXPECT_EQ(e, ecs.getEntity(int(e)));
    EXPECT_THROW(ecs.getEntity(42), Rengine::ECSExceptionEntityNotFound);
}
TEST(ECS, removeEntity)
{
    Rengine::ECS ecs;
    Rengine::Entity& e = ecs.addEntity();

    EXPECT_EQ(e, ecs.getEntity(int(e)));
    ecs.removeEntity(e);
    EXPECT_THROW(ecs.getEntity(int(e)), Rengine::ECSExceptionEntityNotFound);
}
TEST(ECS, registerGetComponent)
{
    Rengine::ECS ecs;

    Rengine::SparseArray<int> &sp = ecs.registerComponent<int>();
    Rengine::SparseArray<int> &sp2 = ecs.getComponents<int>();
    EXPECT_EQ(std::addressof(sp), std::addressof(sp2));
}
static int count = 0;
void componentFunction(const Rengine::ECS&, int&, Rengine::Entity&)
{
    count += 1;
}
TEST(ECS, componentFunction)
{
    Rengine::ECS ecs;
    Rengine::Entity &e = ecs.addEntity();
    Rengine::Entity &e2 = ecs.addEntity();

    ecs.registerComponent<int>();
    e.addComponent<int>(0);
    e2.addComponent<int>(1);
    ecs.setComponentFunction<int>(componentFunction);
    EXPECT_EQ(count, 0);
    ecs.runComponentFunction<int>();
    EXPECT_EQ(count, 2);
}
