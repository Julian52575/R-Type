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
TEST(SparseArray, emplaceAt)
{
    Rengine::SparseArray<int> sp = Rengine::SparseArray<int>(2);

    sp.emplaceAt(0, 0);
    EXPECT_TRUE(sp[0].has_value());
    EXPECT_EQ(sp[0].value(), 0);
    EXPECT_FALSE(sp[1].has_value());
}
TEST(SparseArray, insertAt)
{
    Rengine::SparseArray<int> sp = Rengine::SparseArray<int>(2);

    sp.insertAt(0, 0);
    EXPECT_TRUE(sp[0].has_value());
    EXPECT_EQ(sp[0].value(), 0);
    EXPECT_FALSE(sp[1].has_value());
}
TEST(SparseArray, erase)
{
    Rengine::SparseArray<int> sp = Rengine::SparseArray<int>();

    sp.insertAt(0, 0);
    sp.erase(0);
    EXPECT_FALSE(sp[0].has_value());
}
TEST(SparseArray, operatorBrackets)
{
    Rengine::SparseArray<int> sp = Rengine::SparseArray<int>(2);

    sp[0] = 0;
    EXPECT_TRUE(sp[0].has_value());
    EXPECT_EQ(sp[0].value(), 0);
    sp[0].reset();
    EXPECT_FALSE(sp[0].has_value());
}
TEST(SparseArray, reset)
{
    Rengine::SparseArray<int> sp = Rengine::SparseArray<int>(3);

    sp[2] = 2;
    sp.reset(2);
    EXPECT_FALSE(sp[2].has_value());
}
TEST(SparseArray, addSize)
{
    Rengine::SparseArray<int> sp = Rengine::SparseArray<int>(3);

    sp.addSize(1);
    EXPECT_TRUE(sp.size() == 3 + 1);
}
TEST(SparseArray, getIndex)
{
    Rengine::SparseArray<int> sp = Rengine::SparseArray<int>(3);
    int i = 42;

    sp[0] = 0;
    sp[1] = 1;
    EXPECT_EQ(sp.getIndex(sp[0]), 0);
    EXPECT_EQ(sp.getIndex(sp[1]), 1);
    EXPECT_EQ(sp.getIndex(i), -1);
}
