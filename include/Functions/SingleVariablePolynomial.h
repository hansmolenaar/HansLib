#pragma once

#include "ISingleVariableRealValuedFunction.h"
#include "SingleVariableMonomial.h"

#include <vector>
#include <tuple>

class SingleVariablePolynomial : public ISingleVariableRealValuedFunction
{
public:
	//static SingleVariablePolynomial	Create(std::span<std::tuple<double, SingleVariableMonomial > > polynomial);
	void Add(double coef, SingleVariableMonomial monomial);
	double Evaluate(double x)const override;
	double Derivative(double x)const override;
	bool IsNonConstant() const override;
private:
	std::vector< std::pair<double, SingleVariableMonomial> > m_polynomial;
};
