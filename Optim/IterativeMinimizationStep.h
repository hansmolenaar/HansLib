#pragma once

#include <limits>
#include <vector>

enum IIterativeMinimizationStepStatus
{
    StepSucces,
    StepStuck,
    StepDivergence
};

struct IterativeMinimizationStep
{
    IIterativeMinimizationStepStatus Status = StepDivergence;
    std::vector<double> ArgMinimum;
    double EvalMinimum = std::numeric_limits<double>::max();
};
