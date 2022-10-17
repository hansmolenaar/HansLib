#pragma once

#include "ISingleVariableRealValuedFunction.h"

#include <functional>

class SingleVariableRealValuedFunction : public ISingleVariableRealValuedFunction
{
public:
	SingleVariableRealValuedFunction(std::function<double(double)> function, std::function<double(double)> derivative);
	double Evaluate(double x)const override;
	double Derivative(double x)const override;
	bool IsNonConstant() const override;
private:
	std::function<double(double)> m_function;
	std::function<double(double)> m_derivative;
};
