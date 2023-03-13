#pragma once

#include "MultiVariableRealValuedFunctionEvaluateCached.h"
#include "Interval.h"

struct MinimumInfo
{
   double Extremum = std::numeric_limits<double>::max();
   std::vector<std::vector<double>> Positions;
};

struct MinimizationProblem
{
   std::shared_ptr<MultiVariableRealValuedFunctionEvaluateCached> Function;
   std::vector<Interval<double>> Domain;
   MinimumInfo Minimum;
};

namespace MinimizationExamples
{
   MinimizationProblem Square1D();
   MinimizationProblem Square2D();
}