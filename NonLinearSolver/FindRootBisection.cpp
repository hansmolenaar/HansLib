#include "NonLinearSolver/FindRootBisection.h"

#include "Functions/ISingleVariableRealValuedFunction.h"
#include "NonLinearSolver/INonLinearSolverStrategy.h"
#include "Functions/ISingleVariableRealValuedFunctionUtils.h"
#include "MyAssert.h"

NonLinearSolverStatus FindRootBisection::FindInInterval(ISingleVariableRealValuedFunction& fie, double xmin, double xmax, INonLinearSolverStrategy& strategy, double& result)
{
   const double fmin = ISingleVariableRealValuedFunctionUtils::Evaluate(fie, xmin);
   const double fmax = ISingleVariableRealValuedFunctionUtils::Evaluate(fie, xmax);

   if (fmin > fmax)
   {
      // Swap arguments
      return FindInInterval(fie, xmax, xmin, strategy, result);
   }

   Utilities::MyAssert(fmin <= 0.0);
   Utilities::MyAssert(fmax >= 0.0);

   int iter = 0;
   double rsd = (std::abs(fmin) + std::abs(fmax)) / 2;
   auto status = strategy.GetStatus(iter, { std::addressof(rsd), 1 });

   while (status == NonLinearSolverStatus::NotConverged)
   {
      ++iter;
      result = (xmin + xmax) / 2;
      double fmid = ISingleVariableRealValuedFunctionUtils::Evaluate(fie, result);
      status = strategy.GetStatus(iter, { std::addressof(fmid), 1 });

      if (fmid > 0)
      {
         xmax = result;
      }
      else
      {
         xmin = result;
      }
   }

   return status;
}