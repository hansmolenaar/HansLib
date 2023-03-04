#pragma once
#include "IIterativeMinimizationConvergenceCrit.h"
#include <vector>
#include <limits>

class IIterativeMinimizationConvergenceCrit;
class IIterativeMinimization;

struct IterativeMinimizationResult
{
   IterativeMinimizationConvergenceStatus Status = Fail;
   std::vector<double> ArgMinimum;
   double EvalMinimum = std::numeric_limits<double>::quiet_NaN();
   int NumIterations = -1;
};

namespace IterativeMinimizationController
{
   IterativeMinimizationResult Iterate(IIterativeMinimization&, IIterativeMinimizationConvergenceCrit&);
}