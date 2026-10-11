#include <gtest/gtest.h>

#include "Grid.h"
#include "SweepHiddenSingle.h"

using namespace Sudoku;

TEST(SweepHidddenSingleTest, SweepExample1)
{
   Grid grid;

   grid.setValue(0, 6, 5);
   grid.setValue(1, 0, 1);
   grid.setValue(1, 1, 6);
   grid.setValue(1, 3, 9);
   grid.setValue(2, 2, 9);
   grid.setValue(2, 4, 6);
   grid.setValue(2, 5, 4);
   grid.setValue(3, 8, 4);
   grid.setValue(4, 0, 4);
   grid.setValue(4, 4, 2);
   grid.setValue(4, 6, 1);
   grid.setValue(5, 3, 3);
   grid.setValue(5, 7, 5);
   grid.setValue(6, 2, 2);
   grid.setValue(6, 4, 8);
   grid.setValue(6, 5, 9);
   grid.setValue(7, 1, 1);
   grid.setValue(7, 3, 2);
   grid.setValue(7, 4, 5);
   grid.setValue(7, 7, 3);
   grid.setValue(8, 0, 7);
   grid.setValue(8, 3, 1);
   grid.setValue(8, 8, 9);

   grid.update();
   ASSERT_NO_THROW(grid.checkValidSudoku());
   ASSERT_EQ(grid.getNumCandidates(), 260);

   SweepHiddenSingle().sweep(grid.get());
   ASSERT_EQ(grid.getNumCandidates(), 233);

   grid.update();
   ASSERT_EQ(grid.getNumCandidates(), 152);
}
