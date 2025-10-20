#pragma once

#include "IMultiVariableRealValuedFunction.h"

class MultiVariableVectorComponentFunction : public IMultiVariableRealValuedFunction
{
  public:
    static const MultiVariableVectorComponentFunction &Instance(int ind, int dim);

    int GetDomainDimension() const override;
    double Evaluate(std::span<const double> x) const override;
    void Derivative(std::span<const double> x, std::span<double> dfdx) const override;
    bool DerivativeAlwaysZero(int var) const override;
    bool HasDerivative() const override;

    using IMultiVariableRealValuedFunction::Derivative;
    using IMultiVariableRealValuedFunction::DerivativeAlwaysZero;

  private:
    MultiVariableVectorComponentFunction(int ind, int dim);

    int m_index;
    int m_dim;
};
