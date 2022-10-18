#pragma once

#include "Functions/ISingleVariableRealValuedFunction.h"

class LagrangeInterpolation : ISingleVariableRealValuedFunction
{
public:
   bool HasDerivative() const override;
   bool DerivativeAlwaysZero(int eqn, int var) const override;
   double Evaluate(double x)const override;
   double Derivative(double x)const override;
   bool IsNonConstant() const override;
};
