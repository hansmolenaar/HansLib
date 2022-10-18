#pragma once

#include "ISingleVariableRealValuedFunction.h"

class SingleVariableMonomial : public ISingleVariableRealValuedFunction
{
public:
	SingleVariableMonomial(int pow);
	int GetPower() const;
	double Evaluate(double x)const override;
	double Derivative(double x)const override;
	bool IsNonConstant() const override;
	bool HasDerivative() const override;
private:
	int m_pow;
};
