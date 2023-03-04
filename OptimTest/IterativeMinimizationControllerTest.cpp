#include <gtest/gtest.h>

#include "CompassSearch.h" 
#include "MultiVariableFunctionExamples.h"
#include "Functors.h"
#include "Single.h"
#include "SingleVariablePolynomial.h"
#include "MultiVariableRealValuedFunctionEvaluateCached.h"
#include "IterativeMinimizationController.h"
#include "IIterativeMinimizationConvergenceCrit.h"
//#include "IterativeMinimizationConvergenceCrit.h"

class FakeConvergenceCrit : public IIterativeMinimizationConvergenceCrit
{
public:
   explicit FakeConvergenceCrit(IterativeMinimizationConvergenceStatus status) : m_status(status) {}
   IterativeMinimizationConvergenceStatus getConvergenceStatus(const std::vector<IterativeMinimizationStep>&) const override { return m_status; }
private:
   IterativeMinimizationConvergenceStatus m_status;
};

class FakeIterativeMinimizationStep : public  IIterativeMinimization
{
public:

   FakeIterativeMinimizationStep(IterativeMinimizationStep status, const IMultiVariableFunctionEvaluate& fie) : m_stepResult(status), m_function(fie) {}
   IterativeMinimizationStep iterate() override { return m_stepResult; }
   const IMultiVariableFunctionEvaluate& getObjectiveFunctions() const { return m_function; }
private:
   IterativeMinimizationStep m_stepResult;
   const IMultiVariableFunctionEvaluate& m_function;
};

TEST(IterativeMinimizationControllerTest, CheckLogic)
{
   std::unique_ptr<IMultiVariableFunctionEvaluate> fie = MultiVariableFunctionExamples::GetPolynomial(std::vector< std::pair<std::vector<int>, double>>{ {std::vector<int>{2}, 1.0}});

   FakeConvergenceCrit critConverged = FakeConvergenceCrit(Converged);
   FakeConvergenceCrit  critNotConverged = FakeConvergenceCrit(NotConverged);
   FakeConvergenceCrit critMaxIter = FakeConvergenceCrit(MaxIterExceeded);
   FakeConvergenceCrit critFail = FakeConvergenceCrit(Fail);

   // StepDivergence
   {
      IterativeMinimizationStep step{ StepDivergence };
      FakeIterativeMinimizationStep iterator(step, *fie);

      ASSERT_EQ(IterativeMinimizationController::Iterate(iterator, critConverged).Status, Fail);
      ASSERT_EQ(IterativeMinimizationController::Iterate(iterator, critNotConverged).Status, Fail);
      ASSERT_EQ(IterativeMinimizationController::Iterate(iterator, critMaxIter).Status, Fail);
      ASSERT_EQ(IterativeMinimizationController::Iterate(iterator, critFail).Status, Fail);
   }

   // StepStuck
   {
      IterativeMinimizationStep step{ StepStuck };
      FakeIterativeMinimizationStep iterator(step, *fie);

      ASSERT_EQ(IterativeMinimizationController::Iterate(iterator, critConverged).Status, Converged);
      ASSERT_EQ(IterativeMinimizationController::Iterate(iterator, critNotConverged).Status, MaxIterExceeded);
      ASSERT_EQ(IterativeMinimizationController::Iterate(iterator, critMaxIter).Status, MaxIterExceeded);
      ASSERT_EQ(IterativeMinimizationController::Iterate(iterator, critFail).Status, Fail);
   }

   // StepSucces
   {
      IterativeMinimizationStep step{ StepSucces };
      FakeIterativeMinimizationStep iterator(step, *fie);

      ASSERT_EQ(IterativeMinimizationController::Iterate(iterator, critConverged).Status, Converged);
      // Does not terminate ASSERT_EQ(IterativeMinimizationController::Iterate(iterator, critNotConverged).Status, NotConverged);
      ASSERT_EQ(IterativeMinimizationController::Iterate(iterator, critMaxIter).Status, MaxIterExceeded);
      ASSERT_EQ(IterativeMinimizationController::Iterate(iterator, critFail).Status, Fail);
   }
}

#if false
TEST(IterativeMinimizationControllerTest, MaxIterExceeded)
{
   std::unique_ptr<IMultiVariableFunctionEvaluate> fiePtr = MultiVariableFunctionExamples::GetPolynomial(std::vector< std::pair<std::vector<int>, double>>{ {std::vector<int>{2}, 1.0}});
   std::shared_ptr< IMultiVariableFunctionEvaluate> fie(fiePtr.release());
   CompassSearch cs(fie, std::vector<double> {10}, 1.0);
   IterativeMinimizationConvergenceCrit crit(2, 1.0e-10);
   const auto result = IterativeMinimizationController::Iterate(cs, crit);
   ASSERT_EQ(result.Status, MaxIterExceeded);
}
#endif