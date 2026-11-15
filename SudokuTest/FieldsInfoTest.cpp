#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "FieldsInfo.h"
#include "House.h"
#include "SudokuDefines.h"

using namespace Sudoku;

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
