#pragma once

#include "ISingleVariableRealValuedFunction.h"
#include <memory>

namespace SingleVariableFunctionExamples
{
std::shared_ptr<ISingleVariableRealValuedFunction> GetRungeFunction();
std::shared_ptr<ISingleVariableRealValuedFunction> GetSkewedtHatFunction(double);
std::shared_ptr<ISingleVariableRealValuedFunction> GetStepFunction(double);
std::shared_ptr<ISingleVariableRealValuedFunction> GetDiscontinuousHatFunction(double xPeak, double yPeakLeft,
                                                                               double yPeakRight);
} // namespace SingleVariableFunctionExamples
