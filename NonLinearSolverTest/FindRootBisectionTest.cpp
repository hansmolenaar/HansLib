#include <gtest/gtest.h>

#include "FindRootBisection.h"

#include "NonLinearSolverStrategyResidualReduction.h"
#include "SingleVariableMonomial.h"

const double c_eps = 1.0e-10;

TEST(FindRootBisectionTest, Normal)
{
    SingleVariableMonomial fie(3);
    double result;

    {
        NonLinearSolverStrategyResidualReduction strategy(NonLinearSolverConvergenceCriteria(1.0e-8, 20));
        auto status = FindRootBisection().FindInInterval(fie, -1.0, 2.0, strategy, result);
        ASSERT_EQ(status, NonLinearSolverStatus::Converged);
        ASSERT_NEAR(result, 0.0, 1.0e-2);
    }

    {
        NonLinearSolverStrategyResidualReduction strategy(NonLinearSolverConvergenceCriteria(1.0e-8, 20));
        auto status = FindRootBisection().FindInInterval(fie, 2.0, -1.0, strategy, result);
        ASSERT_EQ(status, NonLinearSolverStatus::Converged);
        ASSERT_NEAR(result, 0.0, 1.0e-2);
    }

    {
        NonLinearSolverStrategyResidualReduction strategy(NonLinearSolverConvergenceCriteria(1.0e-8, 20));
        auto status = FindRootBisection().FindInInterval(fie, 0.0, -1.0, strategy, result);
        ASSERT_EQ(status, NonLinearSolverStatus::Converged);
        ASSERT_NEAR(result, 0.0, 1.0e-2);
    }

    {
        NonLinearSolverStrategyResidualReduction strategy(NonLinearSolverConvergenceCriteria(1.0e-8, 20));
        auto status = FindRootBisection().FindInInterval(fie, 0.0, 1.0, strategy, result);
        ASSERT_EQ(status, NonLinearSolverStatus::Converged);
        ASSERT_NEAR(result, 0.0, 1.0e-2);
    }
}

TEST(FindRootBisectionTest, TestSomeErrors)
{
    SingleVariableMonomial square(2);
    double result;

    {
        NonLinearSolverStrategyResidualReduction strategy(NonLinearSolverConvergenceCriteria(1.0e-5, 20));
        ASSERT_ANY_THROW(FindRootBisection().FindInInterval(square, 1.0, 2.0, strategy, result));
    }

    {
        NonLinearSolverStrategyResidualReduction strategy(NonLinearSolverConvergenceCriteria(1.0e-5, 20));
        ASSERT_ANY_THROW(FindRootBisection().FindInInterval(square, 2.0, 1.0, strategy, result));
    }
    {
        NonLinearSolverStrategyResidualReduction strategy(NonLinearSolverConvergenceCriteria(1.0e-5, 20));
        ASSERT_ANY_THROW(FindRootBisection().FindInInterval(square, 1.0, 1.0, strategy, result));
    }
}
