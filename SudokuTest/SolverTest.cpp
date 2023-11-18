#include <gtest/gtest.h>

#include "Solver.h"
#include "SolverSweep.h"
#include "TestModels.h"


using namespace Sudoku;


TEST(SolverTest, OneStar1)
{
   const Diagram diagramIn = TestModels::getOneStar1();
   const Solver result = Solver::Create(diagramIn);
   ASSERT_TRUE(result.isSolved());

}

TEST(SolverTest, OneStar2)
{
   const Diagram diagramIn = TestModels::getOneStar2();
   const Solver result = Solver::Create(diagramIn);
   const auto str = result.getState().toString();
   ASSERT_TRUE(result.isSolved());
}


TEST(SolverTest, TwoStar1)
{
   const Diagram diagramIn = TestModels::getTwoStar1();
   const Solver result = Solver::Create(diagramIn);
   const auto str = result.getState().toString();
   ASSERT_TRUE(result.isSolved());
}

TEST(SolverTest, ThreeStar1)
{
   const Diagram diagramIn = TestModels::getThreeStar1();
   const Solver result = Solver::Create(diagramIn);
   const auto str = result.getState().toString();
   ASSERT_TRUE(result.isSolved());
}


TEST(SolverTest, ThreeStar2)
{
   const Diagram diagramIn = TestModels::getThreeStar2();
   const Solver result = Solver::Create(diagramIn);
   const auto str = result.getState().toString();
   ASSERT_TRUE(result.isSolved());
}

TEST(SolverTest, FourStar1)
{
   const Diagram diagramIn = TestModels::getFourStar1();
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
