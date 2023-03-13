#include <gtest/gtest.h>

#include "MinimizationProblem.h"
#include "Functors.h"
#include "SparseGridOptimizer.h"
#include "Single.h"
#include "NodeRefinePredicateNever.h"
#include "NodeRefinePredicateAlways.h"
#include "NodeRefinePredicateAdaptive.h"
#include "NodeRefinePredicateFactoryByLevel.h"
#include "IterativeMinimizationConvergenceCritMaxStep.h"
#include "IterativeMinimizationController.h"
#include "Plotting.h"

namespace
{

   void CollocationPointsToFile(std::string functionName, const HierApproximation& approx)
   {
      const size_t dim = approx.getFactory().getDimension();
      std::ofstream ofs(Plotting::GetFile(functionName));
      for (auto d = 0; d < dim; ++d)
      {
         if (d > 0) ofs << " , ";
         ofs << "x" << std::to_string(d);
      }
      ofs << " , y\n";

      for (const auto& xvec : approx.getCollocationPoints())
      {

         for (auto d = 0; d < dim; ++d)
         {
            if (d > 0) ofs << " , ";
            ofs << xvec.at(d);
         }
         ofs << " , " << approx(xvec) << "\n";
      }
      ofs.close();
   }
}

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

   ASSERT_EQ(problem.Function->getNumEvaluations(), 15);

   constexpr Functors::AreClose areClose;
   const double expect = 5.0 / 16.0;
   ASSERT_TRUE(areClose(Utilities::Single(result.ArgBestEval), expect));
}


TEST(SparseGridOptimizerTest, Square1D_Alpha0)
{
   const MinimizationProblem problem = MinimizationExamples::Square1D();
   const double alpha = 0;
   const double fraction = 0.25;
   NodeRefinePredicateFactoryAdaptive predicate(alpha, fraction);
   SparseGridOptimizer optimizer(problem.Function, predicate);
   constexpr int maxLevel = 5;
   IterativeMinimizationConvergenceCritMaxStep maxStep(maxLevel);
   const auto result = IterativeMinimizationController::Iterate(optimizer, maxStep);
   ASSERT_EQ(result.Status, MaxIterExceeded);
   ASSERT_EQ(result.NumIterations, maxLevel);
   const auto& approx = optimizer.getApproximation();
   ASSERT_EQ(approx.getMaxLevel(), 5);
   const auto leafNodes = approx.getLeafNodesRO();
   ASSERT_EQ(approx.getLeafNodesRO().size(), 10);

   ASSERT_EQ(problem.Function->getNumEvaluations(), 19);

   //CollocationPointsToFile("SparseGridOptimizerTest_Square1D_Alpha0", approx);
}


TEST(SparseGridOptimizerTest, Square1D_Alpha1)
{
   const MinimizationProblem problem = MinimizationExamples::Square1D();
   const double alpha = 1;
   const double fraction = 0.25;
   NodeRefinePredicateFactoryAdaptive predicate(alpha, fraction);
   SparseGridOptimizer optimizer(problem.Function, predicate);
   constexpr int maxLevel = 5;
   IterativeMinimizationConvergenceCritMaxStep maxStep(maxLevel);
   const auto result = IterativeMinimizationController::Iterate(optimizer, maxStep);
   const auto& approx = optimizer.getApproximation();
#if false
   ASSERT_EQ(result.Status, MaxIterExceeded);
   ASSERT_EQ(result.NumIterations, maxLevel);
  
   ASSERT_EQ(approx.getMaxLevel(), 5);
   const auto leafNodes = approx.getLeafNodesRO();
   ASSERT_EQ(approx.getLeafNodesRO().size(), 10);

   ASSERT_EQ(problem.Function->getNumEvaluations(), 19);
#endif

   //CollocationPointsToFile("SparseGridOptimizerTest_Square1D_Alpha1", approx);
}

TEST(SparseGridOptimizerTest, PlayGround)
{
   const MinimizationProblem problem = MinimizationExamples::Square2D();
   const double alpha = 0.5;
   const double fraction = 0.10;
   NodeRefinePredicateFactoryAdaptive predicate(alpha, fraction);
   SparseGridOptimizer optimizer(problem.Function, predicate);
   constexpr int maxLevel = 10;
   IterativeMinimizationConvergenceCritMaxStep maxStep(maxLevel);
   const auto result = IterativeMinimizationController::Iterate(optimizer, maxStep);
   const auto& approx = optimizer.getApproximation();

   CollocationPointsToFile("SparseGridOptimizerTest_PlayGround", approx);
}
