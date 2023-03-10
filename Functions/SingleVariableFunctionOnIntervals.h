#pragma once
#include "SingleVariableFunctionOnInterval.h"
#include <memory>

class SingleVariableFunctionOnIntervals : public ISingleVariableRealValuedFunction
{
public:
   SingleVariableFunctionOnIntervals(std::vector<std::shared_ptr<SingleVariableFunctionOnInterval>> fies, double defaultOutsideIntervals = std::numeric_limits<double>::quiet_NaN());

   double Evaluate(double x)const override;
   double Derivative(double x)const override;
   bool IsNonConstant() const override;
   bool HasDerivative() const override;

private:
   std::vector<std::shared_ptr<SingleVariableFunctionOnInterval>> m_intervalFunctions;
   double m_default;
};