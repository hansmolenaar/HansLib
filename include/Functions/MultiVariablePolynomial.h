#pragma once

#include "IMultiVariableRealValuedFunction.h"
#include "MultiVariableMonomial.h"
#include <vector>

class MultiVariablePolynomial : public IMultiVariableRealValuedFunction
{
public:
	explicit MultiVariablePolynomial(int dim);
	void Add(double coef, std::span<const int> monomial);
	void Add(double coef, std::initializer_list< int> monomial);
	int GetDomainDimension() const override;
	double Evaluate(std::span<const double>x)const override;
	void Derivative(std::span<const double>x, std::span< double> dfdx)const override;
	virtual bool DerivativeAlwaysZero(int var) const override;

	using IMultiVariableRealValuedFunction::DerivativeAlwaysZero;
	using IMultiVariableRealValuedFunction::Derivative;
private:
	int m_dim;
	std::vector<std::pair<double, MultiVariableMonomial>> m_terms;
};
