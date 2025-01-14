#include <gtest/gtest.h>
#include <memory>
#include "../src/ComponentRegistry.hpp"

TEST(ComponentRegistry, registerComponent)
{
    Rengine::ComponentRegistry reg;

    EXPECT_NO_THROW(reg.registerComponent<int>());
}
TEST(ComponentRegistry, getComponent)
{
    Rengine::ComponentRegistry reg;

    EXPECT_THROW(reg.getComponents<int>(), Rengine::ComponentRegistryExceptionNotRegistred);
    Rengine::SparseArray<int> &sp = reg.registerComponent<int>();

    EXPECT_EQ(std::addressof(sp), std::addressof(reg.getComponents<int>()));
}
TEST(ComponentRegistry, removeComponent)
{
    Rengine::ComponentRegistry reg;

    reg.registerComponent<int>(0);
    reg.registerComponent<int>(1);
    reg.removeComponent<int>();
    EXPECT_THROW(reg.getComponents<int>(), Rengine::ComponentRegistryExceptionNotRegistred);
}
TEST(ComponentRegistry, clear)
{
    Rengine::ComponentRegistry reg;

    reg.registerComponent<int>(0);
    reg.registerComponent<float>(1);
    reg.clear();
    EXPECT_THROW(reg.getComponents<int>(), Rengine::ComponentRegistryExceptionNotRegistred);
    EXPECT_THROW(reg.getComponents<float>(), Rengine::ComponentRegistryExceptionNotRegistred);
}
