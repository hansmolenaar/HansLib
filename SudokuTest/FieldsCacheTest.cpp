#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "FieldsCache.h"
#include "House.h"
#include "SudokuDefines.h"

using namespace Sudoku;

TEST(FieldsCacheTest, GetHouseFields)
{
   CandidatesGrid grid;
   FieldsCache cache(grid);

   for (const auto &house : House::getAllHouses())
   {
      const FieldBits &fb = cache.getHouseFields(house.houseId);
      for (const auto &cell : house.cells)
      {
         ASSERT_TRUE(fb.test(cell.field));
      }
   }
}
