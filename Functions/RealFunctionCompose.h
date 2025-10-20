#pragma once

#include "IRealFunction.h"

// f: R^k-> R^n
// g: R^m -> R^k
// f*g -> R^m -> R^n
class RealFunctionCompose : public IRealFunction
{
  public:
    RealFunctionCompose(const IRealFunction &fie1, const IRealFunction &fie2);
    int GetRangeDimension() const override;
    int GetDomainDimension() const override;
    void Evaluate(std::span<const double> x, std::span<double> y) const override;
    void Derivative(std::span<const double> x, IMatrix &dfdx) const override;
    bool DerivativeAlwaysZero(int x, int y) const override;
    bool HasDerivative() const override;

  private:
    const IRealFunction &m_f;
    const IRealFunction &m_g;
};
