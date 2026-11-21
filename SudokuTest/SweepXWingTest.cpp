#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "SweepXWing.h"

using namespace Sudoku;

TEST(SweepXWingTest, Basics)
{
   CandidatesGrid grid;

   grid.setCandidates(0, 0, {1, 9});
   grid.setCandidates(0, 8, {1, 9});
   grid.setCandidates(8, 0, {1, 9});
   grid.setCandidates(8, 8, {1, 9});

   for (Index c = 1; c < 8; ++c)
   {
      grid.setGiven(0, c, c + 1);
      Value valueToSet = 9 - c;
      if (valueToSet == 5)
      {
         valueToSet = 6;
      }
      else if (valueToSet == 6)
      {
         valueToSet = 5;
      }
      grid.setGiven(8, c, valueToSet);
   }

   auto expect = 589;
   ASSERT_EQ(expect, grid.getNumCandidates());

   FieldsInfo finfo(grid);
   grid.update(finfo);
   expect -= 142;
   ASSERT_EQ(expect, grid.getNumCandidates());

   auto fieldsValue = SweepXWing().collect(finfo);
   ASSERT_EQ(fieldsValue.size(), 14);
   auto gridChanges = grid.apply(fieldsValue, finfo);
   ASSERT_EQ(gridChanges.numValuesChanged, 14);
   ASSERT_EQ(gridChanges.numFieldsSolved, 0);
   expect -= 14;
   ASSERT_EQ(expect, grid.getNumCandidates());

   fieldsValue = SweepXWing().collect(finfo);
   ASSERT_EQ(fieldsValue.size(), 0);
}
