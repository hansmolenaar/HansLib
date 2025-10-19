#include "FindRootIllinois.h"

#include "ISingleVariableRealValuedFunction.h"
#include "INonLinearSolverStrategy.h"
#include "ISingleVariableRealValuedFunctionUtils.h"
#include "MyAssert.h"

NonLinearSolverStatus FindRootIllinois::FindInInterval(ISingleVariableRealValuedFunction& fie, double a, double b, INonLinearSolverStrategy& strategy, double& result)
{
   int iter = 0;
   double fa = ISingleVariableRealValuedFunctionUtils::Evaluate(fie, a);
   double fb = ISingleVariableRealValuedFunctionUtils::Evaluate(fie, b);
   const double rsd = (fa + fb) / 2;
   auto status = strategy.GetStatus(iter, { std::addressof(rsd), 1 });
   if (status == NonLinearSolverStatus::Converged)
   {
      result = a;
      return status;
   }

   while (status == NonLinearSolverStatus::NotConverged)
   {
      ++iter;
      double x = (a * fb - b * fa) / (fb - fa);
      result = x;
      double fx = ISingleVariableRealValuedFunctionUtils::Evaluate(fie, x);
      status = strategy.GetStatus(iter, { std::addressof(fx), 1 });

      if (status == NonLinearSolverStatus::Converged) break;

      if (fx * fb < 0)
      {
         a = b;
         fa = fb;
      }
      else
      {
         fa *= 0.5;
      }

      b = x;
      fb = fx;
   }

   return status;
}
