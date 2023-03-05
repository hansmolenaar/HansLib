#pragma once

#include "ISingleVariableRealValuedFunction.h"
#include "Interval.h"

#include <memory>

class SingleVariableFunctionOnInterval : public ISingleVariableRealValuedFunction
{
public:
	SingleVariableFunctionOnInterval(std::shared_ptr<ISingleVariableRealValuedFunction>, const Interval<double>& domain);
	const Interval<double>& getDomain() const;

	double Evaluate(double x)const override;
	double Derivative(double x)const override;
	bool IsNonConstant() const override;
	bool HasDerivative() const override;

private:
	std::shared_ptr<ISingleVariableRealValuedFunction> m_function;
	Interval<double> m_domain;
};