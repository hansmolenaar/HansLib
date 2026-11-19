#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "FieldsInfo.h"
#include "House.h"
#include "Single.h"
#include "SudokuDefines.h"
#include "SudokuUtils.h"

using namespace Sudoku;
using namespace Utilities;

TEST(SudokuUtilsTest, Basic)
{
   CandidatesGrid grid;
   grid.setGiven(1, 1);
   grid.setGiven(2, 2);
   FieldBits someCells;
   someCells.set(0);
   someCells.set(2);

   auto retval = SudokuUtils::getUniqueActiveValuesInCells(grid, someCells);
   ASSERT_EQ(retval.count(), 8);

   grid.update();
   retval = SudokuUtils::getUniqueActiveValuesInCells(grid, someCells);
   ASSERT_EQ(retval.count(), 7);

   someCells.reset();
   someCells.set(1);
   someCells.set(2);
   retval = SudokuUtils::getUniqueActiveValuesInCells(grid, someCells);
   ASSERT_EQ(retval.count(), 0);

   someCells.reset();
   someCells.set(0);
   someCells.set(4);
   retval = SudokuUtils::getUniqueActiveValuesInCells(grid, someCells);
   ASSERT_EQ(retval.count(), 7);
}

TEST(SudokuUtilsTest, GetActiveCandidatesInCells)
{
   CandidatesGrid cg;
   FieldsInfo finfo(cg);
   const House house(HouseType::Row, 0);
   const auto houseFields = finfo.getHouseFields(house);
   auto retval = SudokuUtils::getUniqueActiveValuesInCells(cg, houseFields);
   ASSERT_EQ(retval.count(), NumCellsInHouse);

   cg.removeCandidate(0, 9);
   finfo.update();
   retval = SudokuUtils::getUniqueActiveValuesInCells(cg, houseFields);
   ASSERT_EQ(retval.count(), NumCellsInHouse);

   cg.setGiven(1, 5);
   finfo.update();
   retval = SudokuUtils::getUniqueActiveValuesInCells(cg, houseFields);
   ASSERT_EQ(retval.count(), NumCellsInHouse - 1);
}
