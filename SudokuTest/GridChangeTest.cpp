#include <gtest/gtest.h>

#include "GridChange.h"

using namespace Sudoku;

TEST(GridChangeTes, Operator)
{
   GridChange sum;
   const GridChange add{2, 1};
   sum += add;
   sum += add;
   ASSERT_EQ(sum.numValuesChanged, 4);
   ASSERT_EQ(sum.numFieldsSolved, 2);
}
