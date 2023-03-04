#pragma once

#include <vector>
#include <limits>

enum IIterativeMinimizationStepStatus { StepSucces, StepStuck, StepDivergence };

struct IterativeMinimizationStep
{
   IIterativeMinimizationStepStatus Status = StepDivergence;
   std::vector<double> ArgMinimum;
   double EvalMinimum = std::numeric_limits<double>::quiet_NaN();
};
