#pragma once

#include "IRealFunction.h"
#include "IMultiVariableRealValuedFunction.h"

#include <memory>
#include <span>
#include <vector>
#include <functional>

class IMatrix;

class MultiVariableVectorValuedFunction : public IRealFunction
{
public:
	explicit MultiVariableVectorValuedFunction(std::span< std::shared_ptr<IMultiVariableRealValuedFunction> > components);
	int GetRangeDimension() const override;
	int GetDomainDimension() const override;
	void Evaluate(std::span<const double>x, std::span<double> fx)const override;
	void Derivative(std::span<const double>x, IMatrix& dfdx) const override;
	bool DerivativeAlwaysZero(int eqn, int var) const override;
	bool HasDerivative() const override;


private:
	const std::vector<std::shared_ptr< IMultiVariableRealValuedFunction> >  m_components;
};
