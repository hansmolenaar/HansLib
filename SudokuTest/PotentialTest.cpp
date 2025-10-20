#include <gtest/gtest.h>

#include "Potential.h"

using namespace Sudoku;

TEST(PotentialTest, Constructor)
{
    const Potential pot;
    ASSERT_EQ(pot.count(), 9);
    ASSERT_FALSE(Potential::IsSingle(pot));
    for (auto value : ValueAll)
    {
        ASSERT_TRUE(pot.containsValue(value));
    }
}

TEST(PotentialTest, SetAllNone)
{
    Potential pot;

    pot.setAll();
    ASSERT_EQ(pot.count(), NumRowColBox);
    for (auto value : ValueAll)
    {
        ASSERT_TRUE(pot.containsValue(value));
    }

    pot.setNone();
    ASSERT_EQ(pot.count(), 0);
    for (auto value : ValueAll)
    {
        ASSERT_FALSE(pot.containsValue(value));
    }
}

TEST(PotentialTest, SetUnset)
{
    Potential pot;
    pot.setNone();

    pot.set(4);
    ASSERT_EQ(pot.count(), 1);
    ASSERT_TRUE(pot.containsValue(4));
    ASSERT_FALSE(pot.containsValue(6));
    ASSERT_TRUE(Potential::IsSingle(pot));

    pot.set(6);
    ASSERT_EQ(pot.count(), 2);
    ASSERT_TRUE(pot.containsValue(4));
    ASSERT_TRUE(pot.containsValue(6));
    ASSERT_FALSE(Potential::IsSingle(pot));

    pot.unset(4);
    ASSERT_EQ(pot.count(), 1);
    ASSERT_FALSE(pot.containsValue(4));
    ASSERT_TRUE(pot.containsValue(6));

    pot.unset(4);
    ASSERT_EQ(pot.count(), 1);
    ASSERT_FALSE(pot.containsValue(4));
    ASSERT_TRUE(pot.containsValue(6));

    pot.unset(6);
    ASSERT_EQ(pot.count(), 0);
    ASSERT_FALSE(pot.containsValue(4));
    ASSERT_FALSE(pot.containsValue(6));
}

TEST(PotentialTest, getCommon)
{
    Potential pot1;
    Potential pot2;
    pot1.setNone();
    pot2.setNone();
    PotentialValues common = Potential::getIntersection(pot1, pot2);
    ASSERT_TRUE(common.empty());

    pot1.set(2);
    pot2.set(5);
    common = Potential::getIntersection(pot1, pot2);
    ASSERT_TRUE(common.empty());

    pot1.set(5);
    common = Potential::getIntersection(pot1, pot2);
    ASSERT_EQ(common.size(), 1);
    ASSERT_EQ(common.at(0), 5);
}

TEST(PotentialTest, toString)
{
    Potential pot;
    pot.setNone();
    ASSERT_EQ(pot.toString(), "()");

    pot.set(5);
    ASSERT_EQ(pot.toString(), "(5)");

    pot.set(1);
    ASSERT_EQ(pot.toString(), "(1,5)");
}