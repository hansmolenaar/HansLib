#include "Functions/ISingleVariableRealValuedFunctionUtils.h"
#include "Functions/ISingleVariableRealValuedFunction.h"
#include "Utils/NonLinearSolverStrategyResidualReduction.h"
#include "Utils/NonLinearSolver.h"

#include <string>
#include <sstream>
#include <array>


double Evaluate(ISingleVariableRealValuedFunction& fie, double x)
{
	double result;
	fie.Evaluate(std::span<const double>(&x, 1), std::span<double>(&result, 1));
	return result;
}




void CheckDerivative(ISingleVariableRealValuedFunction& fie, double x, double delx, bool isLinear)
{
#if 0
   static int callCount; 
   ++callCount;
   if (callCount == 172)
   {
      int hello = 1;
   }
#endif
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
      MessageHandler::Assert(status == NonLinearSolverStatus::Converged, "Problem with derivative! ");
   }
}

