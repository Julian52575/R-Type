//
#include <gtest/gtest.h>

#include "../../src/Components/Relationship.hpp"

enum Group {
    Group0 = 0b000,
    Group1 = 0b010,
    Group2 = 0b100
};


TEST(Relationship, setGetGroup)
{
    RType::Components::Relationship r(Group0);

    EXPECT_EQ(r.getGroup(), Group0);
    r.setGroup(Group0 | Group2);
    EXPECT_EQ(r.getGroup(), Group0 | Group2);
}
TEST(Relationship, addParent)
{
    RType::Components::Relationship r(Group0);
    uint64_t parent1 = 10;
    uint64_t parent2 = 20;

    r.addParent(parent1);
    EXPECT_FALSE(r.belong(parent1));
    EXPECT_TRUE(r.isRelated(parent1));
    r.addParent(parent2);
    EXPECT_TRUE(r.isRelated(parent2));
    EXPECT_TRUE(r.isParent(parent2));
    EXPECT_FALSE(r.isChild(parent2));
    r.removeParent(parent1);
    EXPECT_FALSE(r.isRelated(parent1));
    EXPECT_FALSE(r.isParent(parent1));
    EXPECT_FALSE(r.isChild(parent1));
}
TEST(Relationship, addChild)
{
    RType::Components::Relationship r(Group0);
    uint64_t child1 = 10;
    uint64_t child2 = 20;

    r.addChild(child1);
    EXPECT_FALSE(r.belong(child1));
    EXPECT_TRUE(r.isRelated(child1));
    r.addChild(child2);
    EXPECT_TRUE(r.isRelated(child2));
    EXPECT_TRUE(r.isChild(child2));
    EXPECT_FALSE(r.isParent(child2));
    r.removeChild(child1);
    EXPECT_FALSE(r.isRelated(child1));
}
