#include <gtest/gtest.h>

#include "LockedPairs.h"

using namespace Sudoku;

TEST(LockedPairTest, Basics)
{
   const LockedPair lp(3, Cell::get(2), Cell::get(1));
   ASSERT_EQ(lp.getValue(), 3);
   ASSERT_EQ(lp.getCells()[0].field, 1);
   ASSERT_EQ(lp.getCells()[1].field, 2);
   ASSERT_EQ(lp, lp);

   const LockedPair other(3, Cell::get(1), Cell::get(3));
   ASSERT_LT(lp, other);
   ASSERT_GT(other, lp);
}
