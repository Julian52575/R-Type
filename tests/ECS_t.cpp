#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <gtest/internal/gtest-port.h>
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
TEST(ECS, removeEntityReference)
{
    Rengine::ECS ecs;
    Rengine::Entity& e = ecs.addEntity();

    EXPECT_EQ(e, ecs.getEntity(int(e)));
    ecs.removeEntity(e);
    EXPECT_THROW(ecs.getEntity(int(e)), Rengine::ECSExceptionEntityNotFound);
}
TEST(ECS, removeEntityIndex)
{
    Rengine::ECS ecs;
    Rengine::Entity& e = ecs.addEntity();
    auto idx = std::size_t(e);

    EXPECT_EQ(e, ecs.getEntity(idx));
    ecs.removeEntity(idx);
    EXPECT_THROW(ecs.removeEntity(idx), Rengine::ECSExceptionEntityNotFound);
}
TEST(ECS, registerGetComponent)
{
    Rengine::ECS ecs;

    Rengine::SparseArray<int> &sp = ecs.registerComponent<int>();
    Rengine::SparseArray<int> &sp2 = ecs.getComponents<int>();
    EXPECT_EQ(std::addressof(sp), std::addressof(sp2));
    EXPECT_THROW(ecs.getComponents<float>(), Rengine::ECSExceptionComponentNotRegistred);
}
static int count = 0;
void componentFunction(Rengine::ECS&, int&, Rengine::Entity&)
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
void componentFunctionWithAdditionalParameters(Rengine::ECS&, int&, Rengine::Entity&, bool& marker, int& count)
{
    count += 1;
    marker = true;
}
void componentFunctionWithLessAditionalParameters(Rengine::ECS&, int&, Rengine::Entity&, int& count)
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
    std::function<void(Rengine::ECS&, int&, Rengine::Entity&, bool&, int&)> fun = componentFunctionWithAdditionalParameters;

    ecs.setComponentFunction<int, bool&, int&>(fun);
    EXPECT_THROW(ecs.runComponentFunction<int>(), Rengine::ECSExceptionBadComponentFunctionType);
    ecs.runComponentFunction<int, bool&, int&>(marker, count);
    EXPECT_TRUE(marker);
    EXPECT_EQ(count, 2);
    std::function<void(Rengine::ECS&, int&, Rengine::Entity&, int&)> funLess = componentFunctionWithLessAditionalParameters;

    ecs.setComponentFunction<int, int&>(funLess);
    ecs.runComponentFunction<int, int&>(count);
    EXPECT_EQ(count, 2 + 2);
}
TEST(ECS, getEntityLimit)
{
    Rengine::ECS::size_type maxEntities = 42;
    Rengine::ECS ecs(maxEntities);

    EXPECT_EQ(ecs.getEntityLimit(), maxEntities);
}
TEST(ECS, getCurrentEntitiesCount)
{
    Rengine::ECS ecs;

    EXPECT_EQ(ecs.getActiveEntitiesCount(), 0);
    Rengine::Entity& e = ecs.addEntity();

    EXPECT_EQ(ecs.getActiveEntitiesCount(), 1);
    ecs.addEntity();
    EXPECT_EQ(ecs.getActiveEntitiesCount(), 2);
    ecs.removeEntity(e);
    EXPECT_EQ(ecs.getActiveEntitiesCount(), 1);
}
TEST(ECS, clearEntities)
{
    Rengine::ECS ecs;
    Rengine::Entity& e0 = ecs.addEntity();
    Rengine::Entity& e1 = ecs.addEntity();

    ecs.clearEntities();
    // References are broken now
    EXPECT_EQ(ecs.getActiveEntitiesCount(), 0);
    EXPECT_THROW(ecs.getEntity(int(e0)), Rengine::ECSExceptionEntityNotFound);
    Rengine::Entity& e0Reborn = ecs.addEntity();

    EXPECT_EQ(int(e0Reborn), int(e0));
}
TEST(ECS, clearComponents)
{
    Rengine::ECS ecs;

    ecs.registerComponent<int>();
    ecs.registerComponent<float>();
    ecs.clearComponents();
    EXPECT_THROW(ecs.getComponents<int>(), Rengine::ECSExceptionComponentNotRegistred);
    EXPECT_THROW(ecs.getComponents<float>(), Rengine::ECSExceptionComponentNotRegistred);
}
TEST(ECS, getHighestEntityId)
{
    Rengine::ECS ecs;

    EXPECT_EQ(ecs.getHighestEntityId(), 0);
    Rengine::Entity& e = ecs.addEntity();

    EXPECT_EQ(ecs.getHighestEntityId(), Rengine::ECS::size_type(e));
    Rengine::Entity& e2 = ecs.addEntity();

    Rengine::ECS::size_type high = Rengine::ECS::size_type(e2) > Rengine::ECS::size_type(e) ? Rengine::ECS::size_type(e2) : Rengine::ECS::size_type(e);

    EXPECT_EQ(ecs.getHighestEntityId(), high);
}
TEST(ECS, doubleRemoveEntity)
{
    Rengine::ECS ecs;
    Rengine::Entity& e = ecs.addEntity();
    auto idx = uint64_t(e);

    EXPECT_NO_THROW(ecs.removeEntity(e));
    EXPECT_THROW(ecs.removeEntity(e), Rengine::ECSExceptionEntityNotFound);
    EXPECT_THROW(ecs.removeEntity(idx), Rengine::ECSExceptionEntityNotFound);
}

void onEntityRemovalFunction(Rengine::Entity&, int& var)
{
    var += 1;
}
TEST(ECS, onEntityRemovalFunction)
{
    Rengine::ECS ecs;
    Rengine::Entity& e = ecs.addEntity();
    Rengine::Entity& e2 = ecs.addEntity();
    int var = 0;
    std::function<void(Rengine::Entity&, int&)> fun(onEntityRemovalFunction);

    ecs.setOnEntityRemovalFunction<int&>(fun);
    ASSERT_NO_THROW(ecs.removeEntity(e));
    ASSERT_EQ(var, 0);  // no onEntityRemovalFunction call without template
    ecs.removeEntity<int&>(e2, var);
    ASSERT_EQ(var, 1);

    // Check highest id
    Rengine::ECS::size_type high = Rengine::ECS::size_type(e2) > Rengine::ECS::size_type(e) ? Rengine::ECS::size_type(e2) : Rengine::ECS::size_type(e);
    Rengine::ECS::size_type low = Rengine::ECS::size_type(e2) < Rengine::ECS::size_type(e) ? Rengine::ECS::size_type(e2) : Rengine::ECS::size_type(e);
    EXPECT_EQ(ecs.getHighestEntityId(), low);
}
TEST(ECS, onEntityRemovalFunctionVoid)
{
    Rengine::ECS ecs;
    Rengine::Entity& e = ecs.addEntity();
    Rengine::Entity& e2 = ecs.addEntity();
    uint64_t var = 0;
    std::function<void(Rengine::Entity&)> fun = [&var](Rengine::Entity&) -> void {
        var += 1;
    };

    ecs.setOnEntityRemovalFunction<>(fun);
    ASSERT_NO_THROW(ecs.removeEntity(e));
    ASSERT_EQ(var, 1);  // no onEntityRemovalFunction call without template
    ecs.removeEntity<>(e2);
    EXPECT_EQ(var, 2);

    // Check highest id
    Rengine::ECS::size_type high = Rengine::ECS::size_type(e2) > Rengine::ECS::size_type(e) ? Rengine::ECS::size_type(e2) : Rengine::ECS::size_type(e);
    Rengine::ECS::size_type low = Rengine::ECS::size_type(e2) < Rengine::ECS::size_type(e) ? Rengine::ECS::size_type(e2) : Rengine::ECS::size_type(e);
    EXPECT_EQ(ecs.getHighestEntityId(), low);
}
TEST(ECS, onEntityRemovalFunctionIndex)
{
    Rengine::ECS ecs;
    Rengine::Entity& e = ecs.addEntity();
    Rengine::Entity& e2 = ecs.addEntity();
    int var = 0;
    std::function<void(Rengine::Entity&, int&)> fun(onEntityRemovalFunction);

    ecs.setOnEntityRemovalFunction<int&>(fun);
    ASSERT_NO_THROW(ecs.removeEntity(uint64_t(e)));
    ASSERT_EQ(var, 0);  // no onEntityRemovalFunction call without template
    ecs.removeEntity<int&>(uint64_t(e2), var);
    ASSERT_EQ(var, 1);

    // Check highest id
    Rengine::ECS::size_type high = Rengine::ECS::size_type(e2) > Rengine::ECS::size_type(e) ? Rengine::ECS::size_type(e2) : Rengine::ECS::size_type(e);
    Rengine::ECS::size_type low = Rengine::ECS::size_type(e2) < Rengine::ECS::size_type(e) ? Rengine::ECS::size_type(e2) : Rengine::ECS::size_type(e);
    EXPECT_EQ(ecs.getHighestEntityId(), low);
}
TEST(ECS, onEntityRemovalFunctionVoidIndex)
{
    Rengine::ECS ecs;
    Rengine::Entity& e = ecs.addEntity();
    Rengine::Entity& e2 = ecs.addEntity();
    uint64_t var = 0;
    std::function<void(Rengine::Entity&)> fun = [&var](Rengine::Entity&) -> void {
        var += 1;
    };

    ecs.setOnEntityRemovalFunction<>(fun);
    ASSERT_NO_THROW(ecs.removeEntity(uint64_t(e)));  // No exception when no template
    EXPECT_EQ(var, 1);
    ecs.removeEntity<>(uint64_t(e2));
    EXPECT_EQ(var, 2);
    // Check highest id
    Rengine::ECS::size_type high = Rengine::ECS::size_type(e2) > Rengine::ECS::size_type(e) ? Rengine::ECS::size_type(e2) : Rengine::ECS::size_type(e);
    Rengine::ECS::size_type low = Rengine::ECS::size_type(e2) < Rengine::ECS::size_type(e) ? Rengine::ECS::size_type(e2) : Rengine::ECS::size_type(e);

    EXPECT_EQ(ecs.getHighestEntityId(), low);
}
TEST(ECS, isEntityActive)
{
    Rengine::ECS ecs;
    Rengine::Entity& e = ecs.addEntity();
    Rengine::Entity& e2 = ecs.addEntity();

    ASSERT_TRUE(ecs.isEntityActive(uint64_t(e)));
    ASSERT_TRUE(ecs.isEntityActive(uint64_t(e2)));
    ecs.removeEntity(e);
    ecs.removeEntity(e2);
    ASSERT_FALSE(ecs.isEntityActive(uint64_t(e)));
    ASSERT_FALSE(ecs.isEntityActive(uint64_t(e2)));
    ASSERT_FALSE(ecs.isEntityActive(-1));
}
