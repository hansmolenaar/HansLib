#pragma once

#include "ISingleVariableRealValuedFunction.h"
#include "MyException.h"

class HatFunction : public ISingleVariableRealValuedFunction
{
  public:
    double Evaluate(double x) const override
    {
        return std::max(1.0 - std::abs(x), 0.0);
    }
    double Derivative(double x) const override
    {
        throw MyException("HatFunction: no derivative");
    }
    bool IsNonConstant() const override
    {
        return true;
    }
    bool HasDerivative() const override
    {
        return false;
    }
};
