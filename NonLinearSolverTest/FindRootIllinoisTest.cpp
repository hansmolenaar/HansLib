#include <gtest/gtest.h>

#include "FindRootIllinois.h"

#include "NonLinearSolverStrategyResidualReduction.h"
#include "SingleVariableMonomial.h"
#include "SingleVariablePolynomial.h"

#include <cmath>

const double c_eps = 1.0e-10;

TEST(FindRootIllinoisTest, Normal)
{
    SingleVariableMonomial fie(3);
    double result;

    {
        NonLinearSolverStrategyResidualReduction strategy(NonLinearSolverConvergenceCriteria(1.0e-8, 20));
        auto status = FindRootIllinois().FindInInterval(fie, -1.0, 2.0, strategy, result);
        ASSERT_EQ(status, NonLinearSolverStatus::Converged);
        ASSERT_NEAR(result, 0.0, 1.0e-2);
    }

    {
        NonLinearSolverStrategyResidualReduction strategy(NonLinearSolverConvergenceCriteria(1.0e-8, 20));
        auto status = FindRootIllinois().FindInInterval(fie, 2.0, -1.0, strategy, result);
        ASSERT_EQ(status, NonLinearSolverStatus::Converged);
        ASSERT_NEAR(result, 0.0, 1.0e-2);
    }
}

TEST(FindRootIllinoisTest, Sqrt2)
{

    SingleVariablePolynomial fie;
    fie.Add(-2.0, 0);
    fie.Add(1.0, 2);

    NonLinearSolverStrategyResidualReduction strategy(NonLinearSolverConvergenceCriteria(1.0e-8, 10));
    double result;
    auto status = FindRootIllinois().FindInInterval(fie, 10.0, 0.0, strategy, result);
    ASSERT_EQ(status, NonLinearSolverStatus::Converged);
    ASSERT_NEAR(result, std::sqrt(2.0), 1.0e-6);
}

TEST(FindRootIllinoisTest, EdgeCases)
{
    SingleVariableMonomial fie(2);

    NonLinearSolverStrategyResidualReduction strategy(NonLinearSolverConvergenceCriteria(1.0e-8, 2));
    double result;
    auto status = FindRootIllinois().FindInInterval(fie, 0.0, 1.0, strategy, result);
    ASSERT_EQ(status, NonLinearSolverStatus::Converged);
    ASSERT_NEAR(result, 0.0, 1.0e-7);

    strategy.Reset();
    status = FindRootIllinois().FindInInterval(fie, -1.0, 0.0, strategy, result);
    ASSERT_EQ(status, NonLinearSolverStatus::Converged);
    ASSERT_NEAR(result, 0.0, 1.0e-7);
}
