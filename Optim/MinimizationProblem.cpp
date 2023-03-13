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

MinimizationProblem MinimizationExamples::Square2D()
{
   const double argmin0 = 1.0 / 3.0;
   const double argmin1 = 1.0 / std::sqrt(2.0);
   MinimizationProblem result;

   auto square = std::make_shared< MultiVariablePolynomial>(2);
   square->Add(1.0, { 2, 0 });
   square->Add(-2.0 / 3.0, { 1, 0 });
   square->Add(1.0, { 0, 2 });
   square->Add(-std::sqrt(2.0), {0, 1});
   square->Add(1.0 / 9.0 + 0.5, { 0, 0 });
   result.Function = std::make_shared<MultiVariableRealValuedFunctionEvaluateCached>(square);

   result.Domain.emplace_back(0.0, 1.0);
   result.Domain.emplace_back(0.0, 1.0);
   result.Minimum.Extremum = 0.0;
   result.Minimum.Positions.emplace_back(std::vector<double>{ argmin0, argmin1});

   return result;
}