#pragma once

#include "IMultiVariableRealValuedFunction.h"
#include <memory>
#include <vector>

namespace MultiVariableFunctionExamples
{
   std::unique_ptr<IMultiVariableRealValuedFunction> GetPolynomial(const std::vector< std::pair<std::vector<int>, double>>&);
}
