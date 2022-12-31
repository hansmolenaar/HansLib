#pragma once

#include "Functions/IMultiVariableRealValuedFunction.h"
#include "HierMultiIndex.h"
#include "Interval/Interval.h"

#include <memory>

class  IHierBasisFunction : public IMultiVariableRealValuedFunction
{
public:

   virtual double operator()(std::span<const double> x) const = 0;
   virtual const HierMultiIndex& getMultiIndex() const = 0;
   virtual Interval<double> getSupportInDirection(size_t) const = 0;

   // Partial implementation IMultiVariableRealValuedFunction
   inline double Evaluate(std::span<const double> x) const override { return (*this)(x); }
   inline void Derivative(std::span<const double>x, std::span< double> dfdx) const override { throw MyException("IHierBasisFunction: no derivative"); }
   inline bool DerivativeAlwaysZero(int var) const override { return false; }
   virtual bool HasDerivative() const override { return false; };
   virtual 	 int GetDomainDimension() const override { return static_cast<int>(getMultiIndex().getDimension()); }
};


class  IHierBasisFunction_Factory
{
public:
   virtual ~IHierBasisFunction_Factory() = default;
   virtual const HierMultiIndex& getLowestLevel() const = 0;
   virtual bool canBeRefined(const HierMultiIndex&) const = 0;
   virtual std::unique_ptr<IHierBasisFunction> create(const HierMultiIndex&) const = 0;
};