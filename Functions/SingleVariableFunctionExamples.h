#pragma once

#include "ISingleVariableRealValuedFunction.h"
#include <memory>

namespace SingleVariableFunctionExamples
{
   std::unique_ptr<ISingleVariableRealValuedFunction> GetRungeFunction();
   std::unique_ptr<ISingleVariableRealValuedFunction> GetSkewedtHatFunction(double);
   std::unique_ptr<ISingleVariableRealValuedFunction> GetStepFunction(double);
   std::unique_ptr<ISingleVariableRealValuedFunction> GetDiscontinuousHatFunction(double xPeak, double yPeakLeft, double yPeakRight);
}
