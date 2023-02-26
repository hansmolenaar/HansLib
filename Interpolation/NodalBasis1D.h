#pragma once

#include "ISingleVariableRealValuedFunction.h"
#include "TartanGrid.h"

class NodalBasis1D : ISingleVariableRealValuedFunction
{
public:
   NodalBasis1D(std::unique_ptr<TartanGrid<double,1>>&&, std::vector<double>&&);
   bool HasDerivative() const override;
   bool DerivativeAlwaysZero(int eqn, int var) const override;
   double Evaluate(double x)const override;
   double Derivative(double x)const override;
   bool IsNonConstant() const override;
private:
   std::unique_ptr<TartanGrid<double, 1>> m_grid;
   std::vector<double> m_coefficients;
};
