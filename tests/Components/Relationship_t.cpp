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

    EXPECT_FALSE(r.belong(parent1));
    r.addParent(parent1);
    EXPECT_TRUE(r.isParented(parent1));
    r.addParent(parent2);
    EXPECT_TRUE(r.isParented(parent2));
    r.removeParent(parent1);
    EXPECT_FALSE(r.isParented(parent1));
}
