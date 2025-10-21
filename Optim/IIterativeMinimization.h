#pragma once

#include "IMultiVariableFunctionEvaluate.h"
#include "IterativeMinimizationStep.h"
#include <limits>
#include <memory>
#include <vector>

class IIterativeMinimization
{
  public:
    virtual IterativeMinimizationStep iterate() = 0;
    virtual const IMultiVariableFunctionEvaluate &getObjectiveFunction() const = 0;
};
