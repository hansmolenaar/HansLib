#pragma once

#include "ISingleVariableRealValuedFunction.h"
#include "MyException.h"

class HatFunction : public ISingleVariableRealValuedFunction
{
  public:
    double operator()(double x) const
    {
        return std::max(1.0 - std::abs(x), 0.0);
    }
    double Evaluate(double x) const override
    {
        return (*this)(x);
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
