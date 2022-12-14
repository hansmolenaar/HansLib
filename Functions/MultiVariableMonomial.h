#pragma once

#include "IMultiVariableRealValuedFunction.h"
#include "SingleVariableMonomial.h"
#include <vector>
#include <initializer_list>

class MultiVariableMonomial : public IMultiVariableRealValuedFunction
{
public:
	explicit MultiVariableMonomial(std::span<const int> powers);
	explicit MultiVariableMonomial(std::initializer_list<int> powers);
	int GetDomainDimension() const override;
	double Evaluate(std::span<const double>x)const override;
	void Derivative(std::span<const double>x, std::span< double> dfdx)const override;
	bool DerivativeAlwaysZero(int var) const override;
	bool HasDerivative() const override;

	using IMultiVariableRealValuedFunction::DerivativeAlwaysZero;
	using IMultiVariableRealValuedFunction::Derivative;
private:
	std::vector<SingleVariableMonomial> m_powers;
};
