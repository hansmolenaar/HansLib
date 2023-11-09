#include <gtest/gtest.h>

#include "Solver.h"

using namespace Sudoku;

#if false
const std::array<Value, NumFields> diagramValues =
{
   0, 0, 0,     0, 0, 0,    0, 0, 0,
   0, 0, 0,     0, 0, 0,    0, 0, 0,
   0, 0, 0,     0, 0, 0,    0, 0, 0,

   0, 0, 0,     0, 0, 0,    0, 0, 0,
   0, 0, 0,     0, 0, 0,    0, 0, 0,
   0, 0, 0,     0, 0, 0,    0, 0, 0,

   0, 0, 0,     0, 0, 0,    0, 0, 0,
   0, 0, 0,     0, 0, 0,    0, 0, 0,
   0, 0, 0,     0, 0, 0,    0, 0, 0,
};
#endif

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
   ASSERT_TRUE(result.isSolved());

}


TEST(SolverTest, OneStar2)
{
   const std::array<Value, NumFields> diagramValues =
   {
   2, 0, 8,     7, 0, 0,    0, 4, 3,
   0, 1, 5,     9, 4, 3,    2, 0, 8,
   9, 0, 0,     2, 0, 0,    7, 1, 5,

   0, 2, 0,     0, 0, 1,    4, 3, 0,
   8, 5, 1,     0, 0, 9,    0, 2, 0,
   4, 3, 0,     0, 0, 7,    8, 5, 0,

   1, 8, 2,     5, 7, 6,    3, 0, 0,
   0, 7, 0,     0, 9, 4,    0, 8, 2,
   3, 9, 4,     1, 8, 2,    5, 0, 6,
   };

   const Diagram diagramIn = Diagram::Create(diagramValues);
   const Solver result = Solver::Create(diagramIn);
   const auto str = result.getState().toString();
   bool tmp = result.getState().isSolved();
   ASSERT_TRUE(result.isSolved());

}