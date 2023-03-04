#pragma once

#include "IMultiVariableFunctionEvaluate.h"
#include "IterativeMinimizationStep.h"
#include <vector>
#include <limits>
#include <memory>



class IIterativeMinimization 
{
public:
	virtual IterativeMinimizationStep iterate() = 0;
	virtual const IMultiVariableFunctionEvaluate& getObjectiveFunctions() const = 0;
};
