#include "RealFunctionCheckDerivative.h"
#include "IMatrixUtils.h"
#include "MatrixDense.h"
#include "NonLinearSolver.h"
#include "NonLinearSolverStrategyResidualReduction.h"
#include "SingleVariableRealValuedFunction.h"

#include <functional>

void RealFunctionCheckDerivative::Check(ISingleVariableRealValuedFunction &fie, double x, double delx, bool isLinear)
{
    const double minDeriv = 1.0e-10;
    const std::array<double, 1> rhs = {fie.Evaluate(x)};
    const double deriv = fie.Derivative(x);
    if (std::abs(deriv) > minDeriv)
    {
        const int maxIter = isLinear ? 1 : 6;
        const double reduction = 1.0e-10;
        NonLinearSolverStrategyResidualReduction strategy(NonLinearSolverConvergenceCriteria(reduction, maxIter));
        std::array<double, 1> sol = {x + delx};
        NonLinearSolverStatus status = NonLinearSolver(fie).Solve(rhs, sol, strategy);
        Utilities::MyAssert(status == NonLinearSolverStatus::Converged, "Problem with derivative!");
    }
    else
    {
        Utilities::MyAssert(false, "Degenerate derivative in starting point");
    }
}

void RealFunctionCheckDerivative::Check(const IRealFunction &system, std::span<const double> x,
                                        std::span<const double> delx)
{
    const int numEqn = system.GetRangeDimension();
    const int numVar = system.GetDomainDimension();
    Utilities::MyAssert(x.size() == numVar);
    Utilities::MyAssert(delx.size() == numVar);
    std::vector<double> y(numEqn);
    MatrixDense jacobian(numEqn, numVar);
    for (int eqn = 0; eqn < numEqn; ++eqn)
    {
        for (int var = 0; var < numVar; ++var)
        {
            std::vector<double> vals(x.begin(), x.end());
            if (system.DerivativeAlwaysZero(eqn, var))
            {
                SetAll(jacobian, 1.0);
                system.Derivative(vals, jacobian);
                Utilities::MyAssert(jacobian(eqn, var) == 0.0);
            }
            else
            {
                std::function<double(double)> evaluate = [&](double xx) {
                    vals[var] = xx;
                    system.Evaluate(vals, y);
                    return y[eqn];
                };
                std::function<double(double)> derivative = [&](double xx) {
                    vals[var] = xx;
                    system.Derivative(vals, jacobian);
                    return jacobian(eqn, var);
                };
                SingleVariableRealValuedFunction fie(evaluate, derivative);
                Check(fie, vals[var], delx[var], false);
            }
        }
    }
}
