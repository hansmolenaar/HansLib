#pragma once

#include "IMultiVariableRealValuedFunction.h"
#include "Interval/Interval.h"
#include <memory>
#include <vector>
#include <functional>

namespace MultiVariableFunctionExamples
{
   std::unique_ptr<IMultiVariableRealValuedFunction> GetPolynomial(const std::vector< std::pair<std::vector<int>, double>>&);
   std::unique_ptr<IMultiVariableRealValuedFunction> SixHumpCamelFunction();
   std::unique_ptr<IMultiVariableRealValuedFunction> ScaleInput(
      std::unique_ptr<IMultiVariableRealValuedFunction>&&, 
      std::unique_ptr<std::function<std::vector<double>(const std::vector<double>&)>>&&);
   std::unique_ptr<IMultiVariableRealValuedFunction> TridFunction(int dim);
   std::vector<Interval<double>> TridBounds(int dim);
}
