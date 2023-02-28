#pragma once

#include "IMultiVariableRealValuedFunction.h"
#include "MultiVariableRealValuedFunctionEvaluateCached.h"
#include "Interval.h"
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
      std::unique_ptr<MultiVariableRealValuedFunctionEvaluateCached> Function;
      std::vector<Interval<double>> Domain;
      std::unique_ptr<ExampleExtremumInfo> Minimum;
      std::unique_ptr<ExampleExtremumInfo> Maximum;
   };


   std::unique_ptr<IMultiVariableRealValuedFunction> GetPolynomial(const std::vector< std::pair<std::vector<int>, double>>&);
   std::unique_ptr<IMultiVariableRealValuedFunction> SixHumpCamelFunction();
   std::unique_ptr<IMultiVariableRealValuedFunction> ScaleInput(
      std::unique_ptr<IMultiVariableRealValuedFunction>&&, 
      std::unique_ptr<std::function<std::vector<double>(const std::vector<double>&)>>&&);

   Examplefunction SkewedHatSquared(const std::vector<double>& pos);
   Examplefunction DiscontinuousHatProduct(const std::vector<double>& xpeak, const std::vector<double>& ypeakLeft, const std::vector<double>& ypeakRight);
   Examplefunction SumOfSquares(int dim);

}
