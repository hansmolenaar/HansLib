#pragma once

#include "IRealFunction.h"


class RealFunctionAdd : public IRealFunction
{
public:
	RealFunctionAdd(const IRealFunction& fie1, const IRealFunction& fie2);
	int GetRangeDimension() const override;
	int GetDomainDimension() const override;
	void Evaluate(std::span<const double>x, std::span< double> y)const override;
	void Derivative(std::span<const double>x, IMatrix& dfdx)const override;
	bool DerivativeAlwaysZero(int x, int y) const override;

private:
	const IRealFunction& m_fie1;
	const IRealFunction& m_fie2;
};
