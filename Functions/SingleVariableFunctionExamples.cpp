#include "Functions/SingleVariableFunctionExamples.h"
#include "Functions/SingleVariableRealValuedFunction.h"
#include "Utilities/BoundsCheck.h"


std::unique_ptr<ISingleVariableRealValuedFunction> SingleVariableFunctionExamples::GetRungeFunction()
{
   return std::make_unique<SingleVariableRealValuedFunction>([](double x) {return 1.0 / (1.0 + 25.0 * x * x); });
}

// f(0) = 0
// f(xpeak) = 1
// f(1) = 0
std::unique_ptr<ISingleVariableRealValuedFunction> SingleVariableFunctionExamples::GetSkewedtHatFunction(double xpeak)
{
   BoundsCheck<double>::Create(0.0, 1.0).check(xpeak);
   return std::make_unique<SingleVariableRealValuedFunction>([xpeak](double x) {
      if (x < 0) return 0.0;
      if (x > 1) return 0.0;
      if (x < xpeak) return x / xpeak;
      return (1 - x) / (1 - xpeak);
      });
}

std::unique_ptr<ISingleVariableRealValuedFunction> SingleVariableFunctionExamples::GetStepFunction(double xstep)
{
   BoundsCheck<double>::Create(0.0, 1.0).check(xstep);
   return std::make_unique<SingleVariableRealValuedFunction>([xstep](double x) {
      if (x < xstep) return 0.0;
      return 1.0;
      });
}

std::unique_ptr<ISingleVariableRealValuedFunction> SingleVariableFunctionExamples::GetDiscontinuousHatFunction(double xPeak, double yPeakLeft, double yPeakRight)
{
   BoundsCheck<double>::Create(0.0, 1.0).check(xPeak);
   return std::make_unique<SingleVariableRealValuedFunction>([xPeak, yPeakLeft, yPeakRight](double x) {
      if (x < 0) return 0.0;
      if (x > 1) return 0.0;
      if (x < xPeak) return yPeakLeft * x / xPeak;
      return (1 - x) / (1 - xPeak) * yPeakRight;
      });
}