#pragma once

#include "ISingleVariableRealValuedFunction.h"
#include <memory>

namespace SingleVariableFunctionExamples
{
   std::unique_ptr<ISingleVariableRealValuedFunction> GetRungeFunction();
}
