#include <gtest/gtest.h>

#include "TestModels.h"

using namespace Sudoku;

TEST(TestModelsTest, FromStrin)
{
   const auto grid =
       TestModels::fromString("1..2..3...2..1..4...3..5..67..6..5...5..8..7...8..4..18..7..4...3..6..2...9..2..7");
   ASSERT_NO_THROW(grid.checkValidSudoku());

   ASSERT_EQ(grid.tryGetSolvedValue(0, 0).value(), 1);
   ASSERT_EQ(grid.tryGetSolvedValue(0, 3).value(), 2);
   ASSERT_EQ(grid.tryGetSolvedValue(8, 8).value(), 7);
}
