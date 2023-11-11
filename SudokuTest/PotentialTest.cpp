#include <gtest/gtest.h>

#include "Potential.h"

using namespace Sudoku;

TEST(PotentialTest, Constructor)
{
   const Potential pot;
   ASSERT_EQ(pot.count(), 9);
   for (auto value : ValueAll)
   {
      ASSERT_TRUE(pot.containsValue(value));
   }
}

TEST(PotentialTest, SetAllNone)
{
   Potential pot;

   pot.setAll();
   ASSERT_EQ(pot.count(), NumRowCol);
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

   pot.set(6);
   ASSERT_EQ(pot.count(), 2);
   ASSERT_TRUE(pot.containsValue(4));
   ASSERT_TRUE(pot.containsValue(6));

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