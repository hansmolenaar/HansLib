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

   auto fields = cih.getFieldsContainingValue(1);
   ASSERT_EQ(Single(fields), 1);
   fields = cih.getFieldsContainingValue(3);
   ASSERT_EQ(fields.size(), 7);
}
