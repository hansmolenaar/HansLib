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
   const HouseId house(HouseType::Row, 0);
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

TEST(SudokuUtilsTest, GetValue2Fields)
{
   CandidatesGrid grid;
   grid.setGiven(0, 1);
   grid.setGiven(8, 8);
   grid.removeCandidate(Cell::rowCol2Field(0, 5), 5);

   std::vector<Field> fields{2};

   auto v2f = SudokuUtils::getValue2Fields(grid, std::span(fields));
   ASSERT_EQ(Utilities::Single(v2f[1]), 2);
   ASSERT_EQ(Utilities::Single(v2f[2]), 2);
   ASSERT_EQ(Utilities::Single(v2f[8]), 2);

   fields.push_back(0);
   v2f = SudokuUtils::getValue2Fields(grid, std::span(fields));
   ASSERT_EQ(v2f[1].size(), 2);
   ASSERT_EQ(v2f[2].size(), 1);
   ASSERT_EQ(v2f[8].size(), 1);

   fields.push_back(8);
   v2f = SudokuUtils::getValue2Fields(grid, std::span(fields));
   ASSERT_EQ(v2f[1].size(), 2);
   ASSERT_EQ(Utilities::Single(v2f[2]), 2);
   ASSERT_EQ(v2f[8].size(), 2);
}
