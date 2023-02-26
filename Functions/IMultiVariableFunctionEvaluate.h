#pragma once

#include "IRealFunction.h"
#include <span>


class IMultiVariableFunctionEvaluate
{
public:
	virtual ~IMultiVariableFunctionEvaluate() noexcept = default;

	virtual int getDimension() const = 0;
	virtual	double operator()(std::span<const double> x) const = 0;
};


