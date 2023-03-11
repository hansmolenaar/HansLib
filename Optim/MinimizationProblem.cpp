#include "MinimizationProblem.h"
#include "MultiVariablePolynomial.h"

#include <cmath>

MinimizationProblem MinimizationExamples::Square1D()
{
   const double argmin = 1.0 / 3.0;
   MinimizationProblem result;

   auto square = std::make_shared< MultiVariablePolynomial>(1);
   square->Add(1.0, { 2 });
   square->Add(-2.0 / 3.0, { 1 });
   square->Add(1.0 / 9.0, { 0 });
   result.Function = std::make_shared<MultiVariableRealValuedFunctionEvaluateCached>(square);

   result.Domain.emplace_back(0.0, 1.0);
   result.Minimum.Extremum = 0.0;
   result.Minimum.Positions.emplace_back(std::vector<double>{ argmin});

   return result;
}