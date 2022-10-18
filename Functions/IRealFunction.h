#pragma once

#include <span>

class IMatrix;

// Maps R^m -> R^n
class IRealFunction
{
public:
	virtual ~IRealFunction() = default;
	virtual int GetRangeDimension() const = 0;
	virtual int GetDomainDimension() const = 0;
	virtual bool HasDerivative() const = 0;
	virtual bool DerivativeAlwaysZero(int eqn, int var) const = 0;
	virtual void Evaluate(std::span<const double>x, std::span< double> y) const = 0;
	virtual void Derivative(std::span<const double>x, IMatrix& dfdx) const = 0;
};
