#pragma once

#include "IMultiVariableRealValuedFunction.h"
#include <vector>

class MultiVariableProductFunction : public IMultiVariableRealValuedFunction
{
public:
	explicit MultiVariableProductFunction(std::initializer_list< const IMultiVariableRealValuedFunction *> functions);
	int GetDomainDimension() const override;
	double Evaluate(std::span<const double>x)const override;
	void Derivative(std::span<const double>x, std::span< double> dfdx)const override;
	bool DerivativeAlwaysZero(int var) const override;

	using IMultiVariableRealValuedFunction::DerivativeAlwaysZero;
	using IMultiVariableRealValuedFunction::Evaluate;
	using IMultiVariableRealValuedFunction::Derivative;
private:
	std::vector< const IMultiVariableRealValuedFunction * > m_functions;
	int m_dim;
};
