#pragma once

#include "IMultiVariableRealValuedFunction.h"
#include "SingleVariableMonomial.h"

#include <vector>
#include <functional>

class MultiVariableRealValuedFunction : public IMultiVariableRealValuedFunction
{
public:
	MultiVariableRealValuedFunction(
		std::vector<bool>  isactive,
		std::function<double(std::span<const double>)>,
		std::function<void(std::span<const double>, std::span< double>)>);

	static MultiVariableRealValuedFunction CreateNull(int numVar);
	static std::shared_ptr<IMultiVariableRealValuedFunction>  CreateNullShared(int numVar);

	int GetDomainDimension() const override;
	double Evaluate(std::span<const double>x) const override;
	void Derivative(std::span<const double>x, std::span< double> dfdx)const override;
	bool DerivativeAlwaysZero(int var) const override;

	using IMultiVariableRealValuedFunction::DerivativeAlwaysZero;
	using IMultiVariableRealValuedFunction::Derivative;
private:
	std::vector<bool> m_isActive;
	std::function<double(std::span<const double>)> m_function;
	std::function<void(std::span<const double>, std::span< double>)> m_derivative;
};

MultiVariableRealValuedFunction operator-(const IMultiVariableRealValuedFunction&, const IMultiVariableRealValuedFunction&);
MultiVariableRealValuedFunction operator/(const IMultiVariableRealValuedFunction&, const IMultiVariableRealValuedFunction&);

