#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "House.h"
#include "SudokuUtils.h"

using namespace Sudoku;

TEST(SudokuUtilsTest, Basic)
{
   CandidatesGrid grid;
   grid.setGiven(1, 1);
   const House house = House::getRow(0);
   const HouseByMultiplicities hbm(grid, house);
   const auto m1 = hbm.get(1);
   ASSERT_EQ(m1.size(), 1);
   ASSERT_EQ(m1.front(), 1);
   const auto m2 = hbm.get(2);
   ASSERT_EQ(m2.size(), 0);
   const auto m9 = hbm.get(9);
   ASSERT_EQ(m9.size(), 8);
}
