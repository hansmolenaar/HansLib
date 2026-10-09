#include <gtest/gtest.h>

#include "Grid.h"

using namespace Sudoku;

TEST(GridTest, DefaultConstructor)
{
   const Grid grid;
   ASSERT_EQ(grid.getNumCandidates(), NumField * NumIndex);
}
