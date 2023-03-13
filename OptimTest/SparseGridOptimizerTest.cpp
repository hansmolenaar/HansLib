#include <gtest/gtest.h>

#include "MinimizationProblem.h"
#include "Functors.h"
#include "SparseGridOptimizer.h"
#include "Single.h"
#include "NodeRefinePredicateNever.h"
#include "NodeRefinePredicateAlways.h"
#include "NodeRefinePredicateFactoryByLevel.h"
#include "IterativeMinimizationConvergenceCritMaxStep.h"
#include "IterativeMinimizationController.h"

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

   const auto step = optimizer.iterate();
   ASSERT_EQ(step.Status, StepSucces);
   constexpr Functors::AreClose areClose;
   ASSERT_TRUE(areClose(Utilities::Single(step.ArgMinimum), 0.25));
   const double expect = 1.0 / 144.0;
   ASSERT_TRUE(areClose((*problem.Function)(step.ArgMinimum), expect));
   ASSERT_TRUE(areClose(step.EvalMinimum, expect));
}




TEST(SparseGridOptimizerTest, Square1D_Iterate)
{
   const MinimizationProblem problem = MinimizationExamples::Square1D();
   NodeRefinePredicateFactoryAlways predicate;
   SparseGridOptimizer optimizer(problem.Function, predicate);
   constexpr int maxLevel = 3;
   IterativeMinimizationConvergenceCritMaxStep maxStep(maxLevel);
   const auto result = IterativeMinimizationController::Iterate(optimizer, maxStep);
   ASSERT_EQ(result.Status, MaxIterExceeded);
   ASSERT_EQ(result.NumIterations, maxLevel);
   const auto& approx = optimizer.getApproximation();
   ASSERT_EQ(approx.getMaxLevel(), maxLevel);
   const auto leafNodes = approx.getLeafNodesRO();
   ASSERT_EQ(approx.getLeafNodesRO().size(), 8);
}