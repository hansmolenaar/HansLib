#include <gtest/gtest.h>

#include "Solver.h"

using namespace Sudoku;

TEST(SolverTest, OneStar1)
{
   const std::array<Value, NumFields> diagramValues =
   {
      4, 9, 6,     0, 0, 2,    1, 8, 5,
      0, 7, 2,     1, 0, 0,    4, 9, 0,
      1, 0, 0,     4, 9, 0,    3, 7, 2,

      9, 4, 0,     6, 2, 7,    8, 5, 1,
      6, 2, 7,     0, 5, 1,    9, 4, 0,
      0, 5, 1,     0, 0, 3,    0, 0, 7,

      0, 0, 4,     0, 3, 9,    5, 1, 8,
      0, 0, 9,     5, 1, 8,    7, 0, 0,
      0, 0, 8,     7, 0, 0,    2, 0, 0,
   };

   const Diagram diagramIn = Diagram::Create(diagramValues);
   const Solver result = Solver::Create(diagramIn);
   const auto str = result.getState().toString();
   bool tmp = result.getState().isSolved();
   ASSERT_TRUE(result.isSolved());

}
