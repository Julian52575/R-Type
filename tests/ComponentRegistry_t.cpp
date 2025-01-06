#include <gtest/gtest.h>
#include <memory>
#include "../src/ComponentRegistry.hpp"

TEST(ComponentRegistry, registerComponent)
{
    Rengine::ComponentRegistry reg;
    Rengine::SparseArray<int> &sp = reg.registerComponent<int>();

    EXPECT_THROW(reg.registerComponent<int>(), Rengine::ComponentRegistryExceptionAlreadyRegistred);
}
TEST(ComponentRegistry, getComponent)
{
    Rengine::ComponentRegistry reg;

    EXPECT_THROW(reg.getComponents<int>(), Rengine::ComponentRegistryExceptionNotRegistred);
    Rengine::SparseArray<int> &sp = reg.registerComponent<int>();

    EXPECT_EQ(std::addressof(sp), std::addressof(reg.getComponents<int>()));
}
