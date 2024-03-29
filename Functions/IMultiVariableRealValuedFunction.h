#pragma once

#include "IRealFunction.h"
#include "IMatrix.h"
#include "IMultiVariableFunctionEvaluate.h"
#include <span>


class IMultiVariableRealValuedFunction : public IRealFunction, public IMultiVariableFunctionEvaluate
{
public:
	virtual ~IMultiVariableRealValuedFunction() = default;

	virtual double Evaluate(std::span<const double> x) const = 0;;
	virtual void Derivative(std::span<const double>x, std::span< double> dfdx) const = 0;
	virtual bool DerivativeAlwaysZero(int var) const = 0;

	int GetRangeDimension() const override;
	void Evaluate(std::span<const double>x, std::span< double> y) const override;
	void Derivative(std::span<const double>x, IMatrix& dfdx) const override;
	bool DerivativeAlwaysZero(int eqn, int var) const override;

	double operator()(std::span<const double> x) const override { return Evaluate(x); }
	int getDimension() const override { return GetDomainDimension(); };
};


