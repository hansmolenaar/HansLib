#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "House.h"
#include "Single.h"
#include "SudokuUtils.h"

using namespace Sudoku;
using namespace Utilities;

TEST(SudokuUtilsTest, Basic)
{
   CandidatesGrid grid;
   grid.setGiven(1, 1);
   grid.setGiven(2, 2);
   grid.update();
   const House house = House::getRow(0);
   const CandidatesInHouse cih(grid, house);
   std::vector<Field> someFields = {0, 2};
   auto retval = cih.getUniqueActiveValuesInCells(someFields);
   ASSERT_EQ(retval.size(), 8);
#if false
   const auto m1 = hbm.get(1);
   ASSERT_EQ(m1.size(), 1);
   ASSERT_EQ(m1.front(), 1);
   const auto m2 = hbm.get(2);
   ASSERT_EQ(m2.size(), 0);
   const auto m9 = hbm.get(9);
   ASSERT_EQ(m9.size(), 8);
#endif
}
