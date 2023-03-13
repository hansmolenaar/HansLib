#include <gtest/gtest.h>

#include "MinimizationProblem.h"
#include "Functors.h"
#include "SparseGridOptimizer.h"
#include "Single.h"
#include "NodeRefinePredicateNever.h"
#include "NodeRefinePredicateAlways.h"


TEST(SparseGridOptimizerTest, Square1D_OnlyLowestLevel)
{
   const MinimizationProblem problem = MinimizationExamples::Square1D();
   NodeRefinePredicateFactoryNever predicate;
   const SparseGridOptimizer optimizer(problem.Function, predicate);
   const auto& steps = optimizer.getAllSteps();
   const auto& step = Utilities::Single(steps);
   ASSERT_EQ(step.Status, StepSucces);

   constexpr Functors::AreClose areClose;
   ASSERT_TRUE(areClose((*problem.Function)(step.ArgMinimum), 1.0 / 36.0));
   ASSERT_TRUE(areClose(step.EvalMinimum, 1.0 / 36.0));
}

TEST(SparseGridOptimizerTest, Square1D_OneLevelOfRefinement)
{
   const MinimizationProblem problem = MinimizationExamples::Square1D();
   NodeRefinePredicateFactoryAlways predicate;
   SparseGridOptimizer optimizer(problem.Function, predicate);

   const auto result = optimizer.iterate();

   //constexpr Functors::AreClose areClose;
   //ASSERT_TRUE(areClose((*problem.Function)(step.ArgMinimum), 1.0 / 36.0));
   //ASSERT_TRUE(areClose(step.EvalMinimum, 1.0 / 36.0));
}


