#pragma once

#include "IMultiVariableRealValuedFunction.h"

class MultiVariableMultiplicativeInverseFunction : public IMultiVariableRealValuedFunction
{
  public:
    explicit MultiVariableMultiplicativeInverseFunction(const IMultiVariableRealValuedFunction &);
    int GetDomainDimension() const override;
    double Evaluate(std::span<const double> x) const override;
    void Derivative(std::span<const double> x, std::span<double> dfdx) const override;
    bool DerivativeAlwaysZero(int var) const override;
    bool HasDerivative() const override;

    using IMultiVariableRealValuedFunction::Derivative;
    using IMultiVariableRealValuedFunction::DerivativeAlwaysZero;
    using IMultiVariableRealValuedFunction::Evaluate;

  private:
    const IMultiVariableRealValuedFunction &m_function;
};
