#include <gtest/gtest.h>

#include "MinimizationProblem.h"
#include "Functors.h"

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

