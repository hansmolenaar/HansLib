#include <gtest/gtest.h>

#include "Grid.h"
#include "SweepHiddenSingle.h"

using namespace Sudoku;

TEST(SweepHidddenSingleTest, SweepExample1)
{
   Grid grid;

   grid.setGiven(0, 6, 5);
   grid.setGiven(1, 0, 1);
   grid.setGiven(1, 1, 6);
   grid.setGiven(1, 3, 9);
   grid.setGiven(2, 2, 9);
   grid.setGiven(2, 4, 6);
   grid.setGiven(2, 5, 4);
   grid.setGiven(3, 8, 4);
   grid.setGiven(4, 0, 4);
   grid.setGiven(4, 4, 2);
   grid.setGiven(4, 6, 1);
   grid.setGiven(5, 3, 3);
   grid.setGiven(5, 7, 5);
   grid.setGiven(6, 2, 2);
   grid.setGiven(6, 4, 8);
   grid.setGiven(6, 5, 9);
   grid.setGiven(7, 1, 1);
   grid.setGiven(7, 3, 2);
   grid.setGiven(7, 4, 5);
   grid.setGiven(7, 7, 3);
   grid.setGiven(8, 0, 7);
   grid.setGiven(8, 3, 1);
   grid.setGiven(8, 8, 9);

   grid.update();
   ASSERT_NO_THROW(grid.checkValidSudoku());
   ASSERT_EQ(grid.getNumCandidates(), 260);

   SweepHiddenSingle().sweep(grid);
   ASSERT_EQ(grid.getNumCandidates(), 233);

   grid.update();
   ASSERT_EQ(grid.getNumCandidates(), 152);
}
