#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "Cell.h"
#include "SweepNakedTuples.h"

using namespace Sudoku;

TEST(SweepNakedTuples, BasicExample)
{
   constexpr Index row = 8;

   CandidatesGrid grid;
   grid.setGiven(row, 1, 6);
   grid.setGiven(row, 3, 4);
   grid.setGiven(row, 4, 2);
   grid.setGiven(row, 7, 8);
   grid.setGiven(row, 8, 9);

   grid.setCandidates(Cell::rowCol2Field(row, 0), {3, 5, 7});
   grid.setCandidates(Cell::rowCol2Field(row, 2), {1, 3});
   grid.setCandidates(Cell::rowCol2Field(row, 5), {1, 5});
   grid.setCandidates(Cell::rowCol2Field(row, 6), {1, 3, 5});

   auto value0 = grid[Cell::rowCol2Field(row, 0)].tryGetSolvedValue();
   ASSERT_FALSE(value0.has_value());
   SweepNakedTuples().sweep(grid);

   value0 = grid[Cell::rowCol2Field(row, 0)].tryGetSolvedValue();
   ASSERT_TRUE(value0.has_value());
   ASSERT_EQ(value0.value(), 7);
}
