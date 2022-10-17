#pragma once

#include "IRealFunction.h"


class RealFunctionNull : public IRealFunction
{
public:
	RealFunctionNull(int numVar, int numEqn);
	int GetRangeDimension() const override;
	int GetDomainDimension() const override;
	void Evaluate(std::span<const double>x, std::span< double> y)const override;
	void Derivative(std::span<const double>x, IMatrix& dfdx)const override;
	bool DerivativeAlwaysZero(int x, int y) const override;

private:
	int m_numEqn;
	int m_numVar;
};
