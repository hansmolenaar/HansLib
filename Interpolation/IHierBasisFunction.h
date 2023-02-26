#pragma once

#include "IMultiVariableRealValuedFunction.h"
#include "HierMultiIndex.h"
#include "Interval.h"

#include <memory>

class IHierBasisFunction1D;

class  IHierBasisFunction : public IMultiVariableRealValuedFunction
{
public:

   virtual double operator()(std::span<const double> x) const = 0;
   virtual const HierMultiIndex& getMultiIndex() const = 0;
   virtual const std::vector<const IHierBasisFunction1D*>& getBasisFunctions1D() const = 0;

   // Partial implementation IMultiVariableRealValuedFunction
   inline double Evaluate(std::span<const double> x) const override { return (*this)(x); }
   inline void Derivative(std::span<const double>x, std::span< double> dfdx) const override { throw MyException("IHierBasisFunction: no derivative"); }
   inline bool DerivativeAlwaysZero(int var) const override { return false; }
   virtual bool HasDerivative() const override { return false; };
   virtual 	 int GetDomainDimension() const override { return static_cast<int>(getMultiIndex().getDimension()); }

   // Helper function
   bool contains(std::span<const double> x) const;
};


class  IHierBasisFunction_Factory
{
public:
   virtual ~IHierBasisFunction_Factory() = default;
   virtual size_t getDimension() const = 0;
   virtual std::vector<HierMultiIndex> getLowestLevel() const = 0;
   virtual bool canBeRefined(const HierMultiIndex&) const = 0;
   virtual const IHierBasisFunction* get(const HierMultiIndex&) = 0;
};