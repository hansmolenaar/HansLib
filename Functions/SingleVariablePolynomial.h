#pragma once

#include "ISingleVariableRealValuedFunction.h"
#include "SingleVariableMonomial.h"

#include <vector>
#include <tuple>

class SingleVariablePolynomial : public ISingleVariableRealValuedFunction
{
public:
   SingleVariablePolynomial() = default;
   SingleVariablePolynomial(std::vector<std::tuple<double, int>> polynomial);
   static SingleVariablePolynomial	Create(std::vector<std::tuple<double, int>> polynomial);
   void Add(double coef, SingleVariableMonomial monomial);
   double Evaluate(double x)const override;
   double Derivative(double x)const override;
   bool IsNonConstant() const override;
   bool HasDerivative() const override;
private:
   std::vector< std::pair<double, SingleVariableMonomial> > m_polynomial;
};
