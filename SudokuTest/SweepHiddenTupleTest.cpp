#include <gtest/gtest.h>

#include "CandidatesGrid.h"
#include "Cell.h"
#include "SweepHiddenTuple.h"

using namespace Sudoku;

TEST(SweepHidddenTupleTest, BasicExample)
{
   CandidatesGrid grid;

   for (Index size = 1; size <= 3; ++size)
   {
      const bool anyChange = SweepHiddenTuple::sweep(grid, size);
      ASSERT_FALSE(anyChange);
   }
}

TEST(SweepHidddenTupleTest, Pair)
{
   CandidatesGrid grid;
   CandidatesGrid prv;

   // Hidden pair 2, 6
   grid.setGiven(3, 1, 1);
   grid.setGiven(5, 1, 5);
   grid.setCandidates(Cell::getFromRowCol(3, 0).field, {3, 4, 7, 8, 9});
   grid.setCandidates(Cell::getFromRowCol(3, 2).field, {3, 4, 7, 9});

   grid.setCandidates(Cell::getFromRowCol(4, 0).field, {2, 3, 4, 6, 7, 8, 9});
   grid.setCandidates(Cell::getFromRowCol(4, 1).field, {3, 4, 7});
   grid.setCandidates(Cell::getFromRowCol(4, 2).field, {4, 7, 8, 9});
   grid.setCandidates(Cell::getFromRowCol(5, 0).field, {2, 3, 6, 7, 9});
   grid.setCandidates(Cell::getFromRowCol(5, 2).field, {4, 3, 7, 9});

   Field expect = 682;
   ASSERT_EQ(grid.getNumCandidates(), expect);

   bool anyChange = SweepHiddenTuple::sweep(grid, 3);
   ASSERT_FALSE(anyChange);

   anyChange = SweepHiddenTuple::sweep(grid, 1);
   ASSERT_FALSE(anyChange);

   anyChange = SweepHiddenTuple::sweep(grid, 2);
   ASSERT_TRUE(anyChange);
   expect -= 8;
   ASSERT_EQ(grid.getNumCandidates(), expect);
}
