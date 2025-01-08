#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <memory>
#include <functional>
#include <string>

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
void componentFunctionWithAdditionalParameters(const Rengine::ECS&, int&, Rengine::Entity&, bool& marker, int& count)
{
    count += 1;
    marker = true;
}
void componentFunctionWithLessAditionalParameters(const Rengine::ECS&, int&, Rengine::Entity&, int& count)
{
    count += 1;
}
TEST(ECS, componentFunctionAdditionalParameters)
{
    Rengine::ECS ecs;
    Rengine::Entity &e = ecs.addEntity();
    Rengine::Entity &e2 = ecs.addEntity();
    bool marker = false;
    int count = 0;

    ecs.registerComponent<int>();
    e.addComponent<int>(0);
    e2.addComponent<int>(1);
    // Creating a std::function is mandatory for parameter template resolution.
    std::function<void(const Rengine::ECS&, int&, Rengine::Entity&, bool&, int&)> fun = componentFunctionWithAdditionalParameters;

    ecs.setComponentFunction<int, bool&, int&>(fun);
    EXPECT_THROW(ecs.runComponentFunction<int>(), Rengine::ECSExceptionBadComponentFunctionType);
    ecs.runComponentFunction<int, bool&, int&>(marker, count);
    EXPECT_TRUE(marker);
    EXPECT_EQ(count, 2);
    std::function<void(const Rengine::ECS&, int&, Rengine::Entity&, int&)> funLess = componentFunctionWithLessAditionalParameters;

    ecs.setComponentFunction<int, int&>(funLess);
    ecs.runComponentFunction<int, int&>(count);
    EXPECT_EQ(count, 2 + 2);
}
