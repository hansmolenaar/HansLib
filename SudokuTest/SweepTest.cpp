#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "FieldsInfo.h"
#include "Sweep.h"
#include "SweepNakedSingle.h"

using namespace Sudoku;

TEST(SweepTest, SingleSweep)
{
   SweepNakedSingle strategy;
   Sweep sweep({&strategy});
   CandidatesGrid grid;
   grid.setGiven(80, 9);

   FieldsInfo finfo(grid);
   const auto changes = sweep.singleSweep(grid, finfo);
   ASSERT_EQ(changes.numValuesChanged, 20);
   ASSERT_EQ(changes.numFieldsSolved, 0);
}

TEST(SweepTest, SingleSweepAndUpdate)
{
   SweepNakedSingle strategy;
   Sweep sweep({&strategy});
   CandidatesGrid grid;
   grid.setCandidates(0, {1});
   grid.setCandidates(8, {1, 2});
   grid.setCandidates(5, {1, 2, 3});

   FieldsInfo finfo(grid);
   const auto anyChange = sweep.singleSweepAndUpdate(grid, finfo);
   ASSERT_TRUE(anyChange);
   ASSERT_EQ(finfo.getSolvedFields().count(), 3);
   ASSERT_EQ(finfo.getActiveFields().count(), 78);
}

TEST(SweepTest, Iterate)
{
   SweepNakedSingle strategy;
   Sweep sweep({&strategy});
   CandidatesGrid grid;
   grid.setCandidates(0, {1});
   grid.setCandidates(8, {1, 2});
   grid.setCandidates(5, {1, 2, 3});
   grid.setCandidates(7, {1, 2, 3, 4});
   grid.setCandidates(3, {1, 2, 3, 4, 5});

   FieldsInfo finfo(grid);
   const auto anyChange = sweep.iterate(grid, finfo);
   ASSERT_TRUE(anyChange);
   ASSERT_EQ(finfo.getSolvedFields().count(), 5);
   ASSERT_EQ(finfo.getActiveFields().count(), 76);
}
