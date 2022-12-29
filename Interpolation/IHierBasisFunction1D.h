#pragma once
#include "HierLevelIndex.h"
#include "Functions/ISingleVariableRealValuedFunction.h"
#include "Utilities/MyException.h"
#include <cstddef>

class  IHierBasisFunction1D : public ISingleVariableRealValuedFunction
{
public:

   virtual double operator()(double x) const = 0;
  
   // Implementation ISingleVariableRealValuedFunction
   inline  double Evaluate(double x)const override { return (*this)(x); }
   inline  double Derivative(double x)const override { throw MyException("IHierBasisFunction1D: no derivative"); }
   inline  bool IsNonConstant() const override { return true; }
   inline  bool HasDerivative() const override { return false; }
};

class  IHierBasisFunction1D_Factory
{
public:
   virtual ~IHierBasisFunction1D_Factory() = default;
   virtual bool hasLevel0() const = 0;
   virtual std::unique_ptr<IHierBasisFunction1D> create(const HierLevelIndex&) = 0;
};