#include <gtest/gtest.h>

#include "Functors.h"
#include "IterativeMinimizationController.h"
#include "IterativeMinimizationConvergenceCritMaxStep.h"
#include "MinimizationProblem.h"
#include "NodeRefinePredicateAdaptive.h"
#include "NodeRefinePredicateAlways.h"
#include "NodeRefinePredicateFactoryByLevel.h"
#include "NodeRefinePredicateNever.h"
#include "Plotting.h"
#include "Single.h"
#include "SparseGridOptimizer.h"

#include <fstream>

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
#if true
   ASSERT_EQ(result.Status, MaxIterExceeded);
   ASSERT_EQ(result.NumIterations, maxLevel);

   ASSERT_EQ(approx.getMaxLevel(), 5);
   const auto leafNodes = approx.getLeafNodesRO();
   ASSERT_EQ(approx.getLeafNodesRO().size(), 32);

   ASSERT_EQ(problem.Function->getNumEvaluations(), 63);
   ASSERT_EQ(approx.getAllTreeNodesRO().size(), 63);
#endif

   //CollocationPointsToFile("SparseGridOptimizerTest_Square1D_Alpha1", approx);
}


TEST(SparseGridOptimizerTest, Square2D_AlphaHalf)
{
   auto problem = MinimizationExamples::Square2D();

   const double alpha = 0.50;
   const double fraction = 1.0 / std::sqrt(101.0); // Avoid nice number: round of to integer in NodeRefineAdaptive
   NodeRefinePredicateFactoryAdaptive predicate(alpha, fraction);
   SparseGridOptimizer optimizer(problem.getFunctionOnUnit(), predicate);
   constexpr int maxLevel = 10;
   IterativeMinimizationConvergenceCritMaxStep maxStep(maxLevel);
   const auto result = IterativeMinimizationController::Iterate(optimizer, maxStep);
   const auto& approx = optimizer.getApproximation();

   // TODO
   //ASSERT_EQ(optimizer.getApproximation().getAllTreeNodesRO().size(), 118);
   //ASSERT_EQ(problem.getOriginalProblem().Function->getNumEvaluations(), 118);
   ASSERT_EQ(optimizer.getApproximation().getAllTreeNodesRO().size(), 117);
   ASSERT_EQ(problem.getOriginalProblem().Function->getNumEvaluations(), 117);

   constexpr Functors::AreClose areCloseEval(1.0e-2, 2.0e-3);
   constexpr Functors::AreClose areCloseArg(2.0e-2);
   ASSERT_TRUE(areCloseEval(result.BestEval, problem.getMinimumOnUnitInfo().Extremum));
   ASSERT_TRUE(areCloseArg(result.ArgBestEval[0], problem.getMinimumOnUnitInfo().Positions.at(0).at(0)));
   ASSERT_TRUE(areCloseArg(result.ArgBestEval[1], problem.getMinimumOnUnitInfo().Positions.at(0).at(1)));
}

TEST(SparseGridOptimizerTest, Camel)
{
   auto problem = MinimizationExamples::SixHumpCamelFunction();

   const double alpha = 0.5;
   const double fraction = 0.1;
   NodeRefinePredicateFactoryAdaptive predicate(alpha, fraction);
   SparseGridOptimizer optimizer(problem.getFunctionOnUnit(), predicate);
   constexpr int maxLevel = 8;
   IterativeMinimizationConvergenceCritMaxStep maxStep(maxLevel);
   const auto result = IterativeMinimizationController::Iterate(optimizer, maxStep);
   const auto& approx = optimizer.getApproximation();

   CollocationPointsToFile("SparseGridOptimizerTest_Camel", approx);

   ASSERT_EQ(optimizer.getApproximation().getAllTreeNodesRO().size(), 67);
   ASSERT_EQ(problem.getOriginalProblem().Function->getNumEvaluations(), 67);

   constexpr Functors::AreClose areCloseEval(5.0e-2);
   ASSERT_TRUE(areCloseEval(result.BestEval, problem.getMinimumOnUnitInfo().Extremum));
}

#if true
TEST(SparseGridOptimizerTest, PlayGround)
{
   auto problem = MinimizationExamples::Rosenbrock2();
   constexpr Functors::AreClose areCloseEval(1.0e-2, 1.0e-8);
   const double checkMinimum = (*problem.getFunctionOnUnit())(problem.getMinimumOnUnitInfo().Positions.front());
   ASSERT_TRUE(Functors::AreClose(1.0e-2, 1.0e-12)(checkMinimum, 0.0));

   const double alpha = 0.1;
   const double fraction = 1.0;
   NodeRefinePredicateFactoryAdaptive predicate(alpha, fraction);
   SparseGridOptimizer optimizer(problem.getFunctionOnUnit(), predicate);
   constexpr int maxLevel = 8;
   IterativeMinimizationConvergenceCritMaxStep maxStep(maxLevel);
   const auto result = IterativeMinimizationController::Iterate(optimizer, maxStep);
   const auto& approx = optimizer.getApproximation();
   const auto numEvaluations = problem.getOriginalProblem().Function->getNumEvaluations();

   //CollocationPointsToFile("SparseGridOptimizerTest_PlayGround", approx);
}
#endif
