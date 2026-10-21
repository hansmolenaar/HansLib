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
   const House house = House::getRow(0);
   const CandidatesInHouse cih(grid, house);
   std::vector<Field> someFields = {0, 2};

   auto retval = SudokuUtils::getUniqueActiveValuesInCells(grid, someFields);
   ASSERT_EQ(retval.size(), 9);

   grid.update();

   retval = SudokuUtils::getUniqueActiveValuesInCells(grid, someFields);
   ASSERT_EQ(retval.size(), 7);

   someFields = {1, 2};
   retval = SudokuUtils::getUniqueActiveValuesInCells(grid, someFields);
   ASSERT_EQ(retval.size(), 0);

   someFields = {0, 4};
   retval = SudokuUtils::getUniqueActiveValuesInCells(grid, someFields);
   ASSERT_EQ(retval.size(), 7);
}
