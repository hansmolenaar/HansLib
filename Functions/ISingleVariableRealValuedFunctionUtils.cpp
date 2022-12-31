#include "Functions/ISingleVariableRealValuedFunctionUtils.h"
#include "Functions/ISingleVariableRealValuedFunction.h"
#include "NonLinearSolver/NonLinearSolverStrategyResidualReduction.h"
#include "NonLinearSolver/NonLinearSolver.h"
#include "Utilities/Assert.h"
#include "Utilities/MyException.h"
#include "Utilities/Plotting.h"

#include <string>
#include <sstream>
#include <array>
#include <filesystem>
#include <fstream>

double ISingleVariableRealValuedFunctionUtils::Evaluate(ISingleVariableRealValuedFunction& fie, double x)
{
   double result;
   fie.Evaluate(std::span<const double>(&x, 1), std::span<double>(&result, 1));
   return result;
}


void ISingleVariableRealValuedFunctionUtils::CheckDerivative(ISingleVariableRealValuedFunction& fie, double x, double delx, bool isLinear)
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
      Utilities::Assert(status == NonLinearSolverStatus::Converged, "Problem with derivative! ");
   }
}

void ISingleVariableRealValuedFunctionUtils::ToFile(const ISingleVariableRealValuedFunction& fie, double xmin, double xmax, int nPoints, const std::string& functionName)
{
   Utilities::Assert(nPoints > 1);
   Utilities::Assert(xmax > xmin);

   std::ofstream ofs(Plotting::GetFile(functionName));
   const double del = (xmax - xmin) / (nPoints - 1);
   ofs << "x" << " , " << "y" << "\n";
   for (int n = 0; n < nPoints; ++n)
   {
      const double x = xmin + n * del;
      const double eval = fie.Evaluate(x);
      ofs << x << " , " << eval << "\n";
   }
   ofs.close();
}


void ISingleVariableRealValuedFunctionUtils::ToFile(const ISingleVariableRealValuedFunction& expect, const ISingleVariableRealValuedFunction& approximate, double xmin, double xmax, int nPoints, const std::string& functionName)
{
   Utilities::Assert(nPoints > 1);
   Utilities::Assert(xmax > xmin);

   std::ofstream ofs(Plotting::GetFile(functionName));
   const double del = (xmax - xmin) / (nPoints - 1);
   ofs << "x , Expect , Actual\n";
   for (int n = 0; n < nPoints; ++n)
   {
      const double x = xmin + n * del;
      const double e = expect.Evaluate(x);
      const double a = approximate.Evaluate(x);
      ofs << x << " , " << e << " , " << a << "\n";
   }
   ofs.close();
}
