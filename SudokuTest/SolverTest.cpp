#include <gtest/gtest.h>

#include "Solver.h"
#include "SolverSweep.h"

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
   ASSERT_TRUE(result.isSolved());
}


TEST(SolverTest, TwoStar1)
{
   const std::array<Value, NumFields> diagramValues =
   {
   0, 3, 0,     0, 8, 7,    4, 6, 5,
   9, 8, 0,     0, 6, 0,    0, 3, 1,
   0, 6, 5,     0, 3, 0,    0, 8, 7,

   0, 0, 0,     1, 7, 0,    6, 5, 4,
   1, 7, 0,     6, 0, 0,    0, 0, 9,
   6, 0, 0,     0, 0, 9,    1, 7, 8,

   8, 1, 2,     7, 0, 0,    5, 0, 0,
   0, 9, 3,     5, 4, 6,    0, 0, 2,
   0, 4, 0,     0, 0, 0,    0, 0, 3,
   };

   const Diagram diagramIn = Diagram::Create(diagramValues);
   const Solver result = Solver::Create(diagramIn);
   const auto str = result.getState().toString();
   ASSERT_TRUE(result.isSolved());
}

TEST(SolverTest, ThreeStar1)
{
   const std::array<Value, NumFields> diagramValues =
   {
   9, 0, 0,     0, 3, 8,    7, 0, 5,
   6, 3, 0,     0, 1, 5,    0, 0, 0,
   7, 1, 0,     9, 4, 0,    0, 3, 0,

   0, 0, 0,     0, 0, 0,    1, 5, 0,
   0, 8, 0,     1, 5, 7,    4, 0, 0,
   0, 0, 0,     0, 0, 0,    0, 0, 3,

   3, 2, 0,     0, 0, 4,    0, 0, 1,
   0, 0, 0,     5, 0, 0,    0, 0, 0,
   0, 0, 1,     3, 0, 9,    0, 6, 4,
   };

   const Diagram diagramIn = Diagram::Create(diagramValues);
   const Solver result = Solver::Create(diagramIn);
   const auto str = result.getState().toString();
   ASSERT_TRUE(result.isSolved());
}


TEST(SolverTest, ThreeStar2)
{
   const std::array<Value, NumFields> diagramValues =
   {
   7, 0, 5,     0, 0, 0,    6, 0, 0,
   0, 9, 0,     6, 3, 0,    0, 0, 5,
   6, 0, 2,     7, 0, 5,    0, 9, 0,

   0, 0, 0,     0, 1, 9,    0, 0, 0,
   0, 0, 0,     3, 2, 0,    0, 7, 0,
   0, 0, 6,     0, 0, 0,    5, 1, 0,

   9, 0, 0,     1, 0, 4,    0, 0, 3,
   0, 0, 4,     2, 6, 0,    9, 5, 7,
   0, 6, 0,     0, 0, 0,    1, 0, 0,
   };

   const Diagram diagramIn = Diagram::Create(diagramValues);
   const Solver result = Solver::Create(diagramIn);
   const auto str = result.getState().toString();
   ASSERT_TRUE(result.isSolved());
}


TEST(SolverTest, FourStar1)
{
   const std::array<Value, NumFields> diagramValues =
   {
   0, 5, 0,     6, 0, 0,    0, 0, 0,
   0, 0, 3,     1, 0, 0,    0, 0, 2,
   0, 0, 0,     0, 5, 0,    6, 7, 3,

   0, 0, 0,     2, 3, 0,    0, 8, 0,
   0, 0, 0,     0, 0, 0,    0, 9, 6,
   4, 0, 1,     0, 0, 0,    0, 0, 0,

   7, 0, 9,     0, 0, 0,    8, 1, 0,
   0, 0, 0,     8, 0, 4,    0, 2, 0,
   0, 1, 0,     0, 0, 9,    0, 0, 0,
   };

   const Diagram diagramIn = Diagram::Create(diagramValues);
   const Solver result = Solver::Create(diagramIn);
   const auto str = result.getState().toString();
   auto potentials = result.getState().getPotentials();
   const auto before = potentials.toString();
   Solver::Solve(potentials);
   const auto allPots = potentials.toString();

   Potentials trial4 = potentials;
   trial4.setSingle(80, 4);
   const bool isSolved4 = Solver::Solve(trial4);
   ASSERT_FALSE(isSolved4);

   Potentials trial5 = potentials;
   trial5.setSingle(80, 5);
   const bool isSolved5 = Solver::Solve(trial5);
   ASSERT_TRUE(isSolved5);
}
