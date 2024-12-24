#include <gtest/gtest.h>
#include "../src/SparseArray.hpp"

TEST(SparseArray, initDefaultSize)
{
    Rengine::SparseArray<int> sp = Rengine::SparseArray<int>();

    EXPECT_EQ(sp.size(), 0);
}
TEST(SparseArray, initUserDefinedSize)
{
    Rengine::SparseArray<int>::size_type size = 12;
    Rengine::SparseArray<int> sp = Rengine::SparseArray<int>(size);

    EXPECT_EQ(sp.size(), size);
    for (Rengine::SparseArray<int>::size_type i = 0; i < sp.size(); i++) {
        EXPECT_FALSE(sp[i].has_value());
    }
}
TEST(SparseArray, addElement)
{
    Rengine::SparseArray<int> sp = Rengine::SparseArray<int>();

    sp.emplaceAt(0, 0);
    EXPECT_TRUE(sp[0].has_value());
    EXPECT_EQ(sp[0].value(), 0);
}
