#include <gtest/gtest.h>

#include "BitsetUtils.h"
#include "CandidatesGrid.h"
#include "FieldsInfo.h"
#include "House.h"
#include "SudokuDefines.h"
#include "TestModels.h"

using namespace Sudoku;
using namespace Utilities;

TEST(FieldsInfoTest, GetHouseFields)
{
   CandidatesGrid grid;
   FieldsInfo finfo(grid);

   for (const auto &house : House::getAllHouses())
   {
      const FieldBits &fb = finfo.getHouseFields(house.houseId);
      for (const auto &cell : house.cells)
      {
         ASSERT_TRUE(fb.test(cell.field));
      }
   }
}

TEST(FieldsInfoTest, GetActiveFieldsWithValue)
{
   CandidatesGrid grid;
   FieldsInfo finfo(grid);
   FieldBits fieldsValue = finfo.getActiveFieldsWithValue(3);
   ASSERT_EQ(fieldsValue.count(), NumField);

   grid = TestModels::getSolvedGrid();
   finfo = FieldsInfo(grid);
   fieldsValue = finfo.getActiveFieldsWithValue(3);
   ASSERT_EQ(fieldsValue.count(), 0);
}

TEST(FieldsInfoTest, GetNotSolvedCells)
{
   CandidatesGrid grid;
   grid.setGiven(42, 4);
   auto finfo = FieldsInfo(grid);
   ASSERT_EQ(finfo.getActiveFields().count(), NumField - 1);

   grid.setGiven(0, 5);
   finfo = FieldsInfo(grid);
   ASSERT_EQ(finfo.getActiveFields().count(), NumField - 2);
   ASSERT_EQ(Bitset{finfo.getActiveFields()}.first(), 1);

   grid = TestModels::getSolvedGrid();
   finfo = FieldsInfo(grid);
   const auto activeCells = finfo.getActiveFields();
   ASSERT_TRUE(activeCells.none());
}

TEST(FieldsInfoTest, GetSolvedCellsBitSet)
{
   CandidatesGrid grid;
   grid.setGiven(42, 4);
   FieldsInfo finfo(grid);
   const auto solvedCells = finfo.getSolvedFields();
   ASSERT_EQ(solvedCells.count(), 1);
   ASSERT_TRUE(solvedCells.test(42));
}
