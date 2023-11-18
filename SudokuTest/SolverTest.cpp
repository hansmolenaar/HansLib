#include <gtest/gtest.h>

#include "Solver.h"
#include "SolverSweep.h"
#include "TestModels.h"


using namespace Sudoku;


TEST(SolverTest, OneStar1)
{
   const Diagram diagramIn = TestModels::getOneStar1();
   const auto result = Solver::Solve(diagramIn);
   ASSERT_TRUE(result.isSolved());
}

TEST(SolverTest, OneStar2)
{
   const Diagram diagramIn = TestModels::getOneStar2();
   const auto result = Solver::Solve(diagramIn);
   const auto str = result.toString();
   ASSERT_TRUE(result.isSolved());
}


TEST(SolverTest, TwoStar1)
{
   const Diagram diagramIn = TestModels::getTwoStar1();
   const auto result = Solver::Solve(diagramIn);
   const auto str = result.toString();
   ASSERT_TRUE(result.isSolved());
}

TEST(SolverTest, ThreeStar1)
{
   const Diagram diagramIn = TestModels::getThreeStar1();
   const auto result = Solver::Solve(diagramIn);
   const auto str = result.toString();
   ASSERT_TRUE(result.isSolved());
}


TEST(SolverTest, ThreeStar2)
{
   const Diagram diagramIn = TestModels::getThreeStar2();
   const auto result = Solver::Solve(diagramIn);
   const auto str = result.toString();
   ASSERT_TRUE(result.isSolved());
}

static Potentials SetPairGraph(const Potentials& potentials, Value value)
{
   Potentials result;
   result.setNone();
   for (FieldIndex f = 0; f < NumFields; ++f)
   {
      const auto& pot = potentials.get(f);
      if (pot.count() == 2)
      {
         PotentialValues potentialValues = pot.getPotentialValues();
         if (potentialValues.front() == value)
         {
            result.setSingle(f, potentialValues.back());
         }
         else if (potentialValues.back() == value)
         {
            result.setSingle(f, potentialValues.front());
         }
      }
   }
   return result;
}

TEST(SolverTest, FourStar1)
{
   const Diagram diagramIn = TestModels::getFourStar1();

   auto potentials = diagramIn.getPotentials();
   const auto before = potentials.toString();
   ASSERT_EQ(potentials.getNumSingles(), 25);

   Solver::Solve(potentials);

   const auto allPots = potentials.toString();
   ASSERT_EQ(potentials.getNumSingles(), 43);

   auto pairGraph2 = SetPairGraph(potentials, 2);
   const auto pairGraph2String = pairGraph2.toString();

   auto pairGraph5 = SetPairGraph(potentials, 5);
   const auto pairGraph5String = pairGraph5.toString();

   Potentials trial4 = potentials;
   trial4.setSingle(80, 4);
   const bool isSolved4 = Solver::Solve(trial4);
   ASSERT_EQ(trial4.getNumSingles(), 74);
   ASSERT_FALSE(isSolved4);

   Potentials trial5 = potentials;
   trial5.setSingle(80, 5);
   const bool isSolved5 = Solver::Solve(trial5);
   ASSERT_EQ(trial5.getNumSingles(), NumFields);
   ASSERT_TRUE(isSolved5);
}


TEST(SolverTest, FourStar2)
{
   const Diagram diagramIn = TestModels::getFourStar2();

   auto potentials = diagramIn.getPotentials();
   const auto before = potentials.toString();
   ASSERT_EQ(potentials.getNumSingles(), 24);

   const auto result = Solver::Solve(diagramIn);

   potentials = result.getPotentials();
   const auto allPots = potentials.toString();
   ASSERT_EQ(potentials.getNumSingles(), NumFields);
}


TEST(SolverTest, FourStar3)
{
   const Diagram diagram = TestModels::getFourStar3();

   auto potentials = diagram.getPotentials();
   const auto before = potentials.toString();
   ASSERT_EQ(potentials.getNumSingles(), 25);

   Solver::Solve(potentials);

   const auto after = potentials.toString();
   ASSERT_EQ(potentials.getNumSingles(), 81);
}