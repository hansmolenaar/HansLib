#include <gtest/gtest.h>

#include "CompassSearch.h" 
#include "MultiVariableFunctionExamples.h"
#include "Functors.h"
#include "Single.h"
#include "SingleVariablePolynomial.h"
#include "MultiVariableRealValuedFunctionEvaluateCached.h"
#include "IterativeMinimizationController.h"
#include "IIterativeMinimizationConvergenceCrit.h"

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
   IterativeMinimizationStep step{ StepDivergence };
   FakeIterativeMinimizationStep iterator(step, *fie);
   FakeConvergenceCrit crit(Fail);
   auto retval = IterativeMinimizationController::Iterate(iterator, crit);
   ASSERT_EQ(retval.Status, Fail);
}