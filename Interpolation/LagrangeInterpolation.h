#pragma once

#include "Functions/ISingleVariableRealValuedFunction.h"

class LagrangeInterpolation : ISingleVariableRealValuedFunction
{
public:
   LagrangeInterpolation(std::span<const double>x, std::span<const double>y);
   bool HasDerivative() const override;
   bool DerivativeAlwaysZero(int eqn, int var) const override;
   double Evaluate(double x)const override;
   double Derivative(double x)const override;
   bool IsNonConstant() const override;
private:
   std::vector<double> m_xvals;
   std::vector<double> m_yvals;
};
