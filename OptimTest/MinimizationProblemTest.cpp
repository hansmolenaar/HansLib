#include <gtest/gtest.h>

#include "MinimizationProblem.h"
#include "Functors.h"
#include "Single.h"

namespace
{
   void CheckProblem(const MinimizationProblem& problem, const Functors::AreClose& areClose)
   {
      ASSERT_TRUE(problem.Function);
      ASSERT_TRUE(!problem.Domain.empty());
      ASSERT_EQ(problem.Domain.size(), problem.Function->getDimension());
      ASSERT_TRUE(!problem.Minimum.Positions.empty());
      for (const auto& pos : problem.Minimum.Positions)
      {
         ASSERT_EQ(pos.size(), problem.Domain.size());
         const double eval = (*problem.Function)(pos);
         ASSERT_TRUE(areClose(problem.Minimum.Extremum, eval));
      }
   }
}

TEST(MinimizationProblemTest, Square1D)
{
   const MinimizationProblem problem = MinimizationExamples::Square1D();
   constexpr Functors::AreClose areClose;
   CheckProblem(problem, areClose);
   ASSERT_TRUE(areClose((*problem.Function)(std::vector<double>{0}), 1.0/9.0));
   ASSERT_TRUE(areClose((*problem.Function)(std::vector<double>{1}), 4.0 / 9.0));
}


TEST(MinimizationProblemTest, Square2D)
{
   const auto problem = MinimizationExamples::Square2D();
   constexpr Functors::AreClose areClose;
   CheckProblem(problem.getOriginalProblem(), areClose);
   const auto& function01 = *problem.getFunctionOnUnit();
   ASSERT_TRUE(areClose(function01(std::vector<double>{0,0}), 5.0));
   ASSERT_TRUE(areClose(function01(std::vector<double>{1, 0}), 8.0));
   ASSERT_TRUE(areClose(function01(std::vector<double>{0, 1}), 10.0));
   ASSERT_TRUE(areClose(function01(std::vector<double>{1, 1}), 13.0));
   ASSERT_TRUE(areClose(problem.getMinimumOnUnitInfo().Extremum, 0.0));
   ASSERT_TRUE(areClose(Utilities::Single( problem.getMinimumOnUnitInfo().Positions).at(0), 1.0/3.0));
   ASSERT_TRUE(areClose(Utilities::Single(problem.getMinimumOnUnitInfo().Positions).at(1), 2.0 / 5.0));
}
