#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "StrategyXWing.h"
#include "TestUtils.h"

using namespace Sudoku;

TEST(StrategyXWingTest, Strategy)
{
   StrategyXWing strategy;
   TestUtils::CheckStrategyInterface(strategy);
}

TEST(StrategyXWingTest, Basics)
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

   StrategyXWing strategy;
   // TestUtils::ShowStrategy(strategy, grid);
   TestUtils::CheckStrategy(strategy, grid, {14, 0});

   FieldsInfo finfo(grid);
   grid.update(finfo);
   expect -= 142;
   ASSERT_EQ(expect, grid.getNumCandidates());

   FieldValues fieldValues;
   StrategyXWing().collect(finfo, fieldValues);
   ASSERT_EQ(fieldValues.size(), 14);
   auto gridChanges = grid.apply(fieldValues, finfo);
   ASSERT_EQ(gridChanges.numValuesChanged, 14);
   ASSERT_EQ(gridChanges.numFieldsSolved, 0);
   expect -= 14;
   ASSERT_EQ(expect, grid.getNumCandidates());

   fieldValues = FieldValues();
   StrategyXWing().collect(finfo, fieldValues);
   ASSERT_EQ(fieldValues.size(), 0);
}
