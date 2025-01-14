#include <functional>
#include <gtest/gtest.h>
#include <memory>
#include "../src/ECS.hpp"

TEST(Entity, addComponent)
{
    Rengine::ECS ecs;
    Rengine::Entity &e0 = ecs.addEntity();
    // Exception when component not registered in ECS
    EXPECT_THROW(e0.addComponent<int>(0), Rengine::ComponentRegistryExceptionNotRegistred);
    Rengine::SparseArray<int>& sp = ecs.registerComponent<int>();

    int& con = e0.addComponent<int>(0);
    EXPECT_TRUE(sp[int(e0)].has_value());
    EXPECT_EQ(std::addressof(sp[int(e0)].value()), std::addressof(con));
}
TEST(Entity, removeComponent)
{
    Rengine::ECS ecs;
    Rengine::Entity &e = ecs.addEntity();
    Rengine::SparseArray<int>& sp = ecs.registerComponent<int>();
    int value = 42;
    int& con = e.addComponent<int>(42);

    EXPECT_TRUE(sp[Rengine::ECS::size_type(e)].has_value());
    EXPECT_EQ(sp[Rengine::ECS::size_type(e)].value(), value);
    e.removeComponent<int>();
    EXPECT_THROW(e.getComponent<int>(), Rengine::EntityExceptionComponentNotLinked);
    EXPECT_FALSE(sp[Rengine::ECS::size_type(e)].has_value());
}
TEST(Entity, removeComponentNoExcept)
{
    Rengine::ECS ecs;
    Rengine::Entity &e = ecs.addEntity();
    Rengine::SparseArray<int>& sp = ecs.registerComponent<int>();
    int value = 42;
    int& con = e.addComponent<int>(value);

    EXPECT_TRUE(sp[Rengine::ECS::size_type(e)].has_value());
    EXPECT_EQ(sp[Rengine::ECS::size_type(e)].value(), value);
    e.removeComponent<int>();
    EXPECT_NO_THROW(e.removeComponentNoExcept<int>());
    EXPECT_FALSE(sp[Rengine::ECS::size_type(e)].has_value());
}
TEST(Entity, getComponent)
{
    Rengine::ECS ecs;
    Rengine::Entity &e = ecs.addEntity();
    int value = 42;

    ecs.registerComponent<int>();
    EXPECT_THROW(e.getComponent<int>(), Rengine::EntityExceptionComponentNotLinked);
    int& con = e.addComponent<int>(value);

    EXPECT_EQ(con, value);
    EXPECT_EQ(std::addressof(con), std::addressof(e.getComponent<int>()));
}
static int entityTmp = 0;
void byeByeComponents(Rengine::Entity& e)
{
    entityTmp = int(e);
    e.removeComponentNoExcept<int>();
    e.removeComponentNoExcept<float>();
}
TEST(Entity, destroyComponents)
{
    Rengine::ECS ecs;
    Rengine::Entity &e = ecs.addEntity();

    ecs.registerComponent<int>();
    ecs.registerComponent<float>();
    e.addComponent<int>(0);
    e.setComponentsDestroyFunction(byeByeComponents);
    e.destroyComponents();
    EXPECT_THROW(e.getComponent<int>(), Rengine::EntityExceptionComponentNotLinked);
}
TEST(Entity, destroyComponentsWithUserProvidedFunction)
{
    Rengine::ECS ecs;
    Rengine::Entity &e = ecs.addEntity();

    ecs.registerComponent<int>();
    e.addComponent<int>(0);
    e.setComponentsDestroyFunction(byeByeComponents);
    entityTmp = int(e) + 1;
    e.destroyComponents();
    EXPECT_THROW(e.getComponent<int>(), Rengine::EntityExceptionComponentNotLinked);
    EXPECT_EQ(entityTmp, int(e));
    // Ensure component at index int(entity) was destroyed
    EXPECT_FALSE(ecs.getComponents<int>()[int(e)].has_value());
}
TEST(Entity, setGetflag)
{
    Rengine::ECS ecs;
    Rengine::Entity &e = ecs.addEntity();
    uint64_t flag = 12;

    e.setFlag(flag);
    EXPECT_EQ(e.getFlag(), flag);
}
TEST(Entity, getComponentNoExcept)
{
    Rengine::ECS ecs;
    Rengine::Entity& e = ecs.addEntity();

    ecs.registerComponent<int>();
    int& comp = e.addComponent<int>(0);
    std::optional<std::reference_wrapper<int>> comp2 = e.getComponentNoExcept<int>();

    EXPECT_TRUE(comp2.has_value());
    EXPECT_EQ(std::addressof(comp), std::addressof(comp2.value().get()));
}
