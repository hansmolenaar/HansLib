#pragma once

#include "IMultiVariableRealValuedFunction.h"
#include "Interval/Interval.h"
#include <memory>
#include <vector>
#include <functional>
#include <limits>

namespace MultiVariableFunctionExamples
{
   struct ExampleExtremumInfo
   {
      double Extremum = std::numeric_limits<double>::quiet_NaN();
      std::vector<std::vector<double>> Positions;
   };

   struct Examplefunction
   {
      std::unique_ptr<IMultiVariableRealValuedFunction> Function;
      std::vector<Interval<double>> Domain;
      std::unique_ptr<ExampleExtremumInfo> Minimum;
      std::unique_ptr<ExampleExtremumInfo> Maximum;
   };


   std::unique_ptr<IMultiVariableRealValuedFunction> GetPolynomial(const std::vector< std::pair<std::vector<int>, double>>&);
   std::unique_ptr<IMultiVariableRealValuedFunction> SixHumpCamelFunction();
   std::unique_ptr<IMultiVariableRealValuedFunction> ScaleInput(
      std::unique_ptr<IMultiVariableRealValuedFunction>&&, 
      std::unique_ptr<std::function<std::vector<double>(const std::vector<double>&)>>&&);
   std::unique_ptr<IMultiVariableRealValuedFunction> TridFunction(int dim);
   std::vector<Interval<double>> TridBounds(int dim);

   Examplefunction SkewedHatSquared(const std::vector<double>& pos);
}
