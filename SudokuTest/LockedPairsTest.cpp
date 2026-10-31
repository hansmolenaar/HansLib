#include <gtest/gtest.h>

#include "CandidatesGrid.h"
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

TEST(LockedPairsTest, Basics)
{
   const std::string puzzle = ".2.9......48....31....63.2...94.7..3..3.8.2..4..1.56...3.57....25....18......6.5.";

   auto grid = CandidatesGrid::FromString(puzzle);
   grid.update();
   LockedPairs lps(grid);
   ASSERT_EQ(lps.getPairs(1).size(), 4);
   ASSERT_EQ(lps.getPairs(2).size(), 0);
   ASSERT_EQ(lps.getPairs(3).size(), 0);
   ASSERT_EQ(lps.getPairs(4).size(), 2);
   ASSERT_EQ(lps.getPairs(5).size(), 5);
   ASSERT_EQ(lps.getPairs(6).size(), 4);
   ASSERT_EQ(lps.getPairs(7).size(), 1);
   ASSERT_EQ(lps.getPairs(8).size(), 7);
   ASSERT_EQ(lps.getPairs(9).size(), 4);
}
