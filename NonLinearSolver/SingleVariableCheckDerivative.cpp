#include "SingleVariableCheckDerivative.h"
#include "NonLinearSolverStrategyResidualReduction.h"
#include "NonLinearSolver.h"

void NsNonLinearSolver::SingleVariableCheckDerivative(ISingleVariableRealValuedFunction& fie, double x, double delx, bool isLinear)
{
   const double minDeriv = 1.0e-10;
   const  std::array<double, 1> rhs = { fie.Evaluate(x) };
   const double deriv = fie.Derivative(x);
   if (std::abs(deriv) > minDeriv)
   {
      const int maxIter = isLinear ? 1 : 6;
      const double reduction = 1.0e-10;
      NonLinearSolverStrategyResidualReduction strategy(NonLinearSolverConvergenceCriteria(reduction, maxIter));
      std::array<double, 1> sol = { x + delx };
      NonLinearSolverStatus status = NonLinearSolver(fie).Solve(rhs, sol, strategy);
      Utilities::MyAssert(status == NonLinearSolverStatus::Converged, "Problem with derivative! ");
   }
   else
   {
      Utilities::MyAssert(false, "Degenerate derivative in starting point");
   }
}
