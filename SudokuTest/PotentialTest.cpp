#include <gtest/gtest.h>

#include "Potential.h"

using namespace Sudoku;

TEST(PotentialTest, Constructor)
{
   const Potential pot;
   ASSERT_EQ(pot.Count(), 0);
   for (auto index : RowColAll)
   {
      ASSERT_FALSE(pot(index));
   }
}

TEST(PotentialTest, SetAllNone)
{
   Potential pot;

   pot.SetAll();
   ASSERT_EQ(pot.Count(), NumRowCol);
   for (auto index : RowColAll)
   {
      ASSERT_TRUE(pot(index));
   }

   pot.SetNone();
   ASSERT_EQ(pot.Count(), 0);
   for (auto index : RowColAll)
   {
      ASSERT_FALSE(pot(index));
   }
}


TEST(PotentialTest, SetUnset)
{
   Potential pot;

   pot.Set(4);
   ASSERT_EQ(pot.Count(), 1);
   ASSERT_TRUE(pot(4));
   ASSERT_FALSE(pot(6));

   pot.Set(6);
   ASSERT_EQ(pot.Count(), 2);
   ASSERT_TRUE(pot(4));
   ASSERT_TRUE(pot(6));

   pot.Unset(4);
   ASSERT_EQ(pot.Count(), 1);
   ASSERT_FALSE(pot(4));
   ASSERT_TRUE(pot(6));

   pot.Unset(4);
   ASSERT_EQ(pot.Count(), 1);
   ASSERT_FALSE(pot(4));
   ASSERT_TRUE(pot(6));

   pot.Unset(6);
   ASSERT_EQ(pot.Count(), 0);
   ASSERT_FALSE(pot(4));
   ASSERT_FALSE(pot(6));
}


TEST(PotentialTest, Combine)
{
   constexpr RowColIndex test = 5;
   Potential pot1;
   Potential pot2;
   Potential pot3;

   pot1.SetNone(); pot1.Set(test);
   pot2.SetAll();
   pot3.SetAll();
   Potential result = Potential::Combine(pot1, pot2, pot3);
   ASSERT_EQ(result.Count(), 1);
   ASSERT_TRUE(result(test));

   pot1.SetAll();
   pot2.SetNone(); pot2.Set(test);
   pot3.SetAll();
   result = Potential::Combine(pot1, pot2, pot3);
   ASSERT_EQ(result.Count(), 1);
   ASSERT_TRUE(result(test));

   pot1.SetAll();
   pot2.SetAll();
   pot3.SetNone(); pot3.Set(test);

   result = Potential::Combine(pot1, pot2, pot3);
   ASSERT_EQ(result.Count(), 1);
   ASSERT_TRUE(result(test));
}