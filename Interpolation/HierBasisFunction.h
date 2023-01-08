#pragma once

#include "IHierBasisFunction.h"
#include "IHierBasisFunction1D.h"

class HierBasisFunction : public IHierBasisFunction
{
public:
   explicit HierBasisFunction(const std::vector<const IHierBasisFunction1D*>&);

   double operator()(std::span<const double> x) const override;
   const HierMultiIndex& getMultiIndex() const override;
   const std::vector<const IHierBasisFunction1D*>& getBasisFunctions1D() const override;

private:
   std::vector<const IHierBasisFunction1D*> m_basisFunctions;
   HierMultiIndex m_multiIndex;
};



class  HierBasisFunction_Factory : public IHierBasisFunction_Factory
{
public:
   HierBasisFunction_Factory(size_t, const IHierBasisFunction1D_Factory*);
   size_t getDimension() const override;
   std::vector<HierMultiIndex> getLowestLevel() const override;
   bool canBeRefined(const HierMultiIndex&) const override;
   const IHierBasisFunction* get(const HierMultiIndex&) const override;
private:
   size_t m_dimension;
   const IHierBasisFunction1D_Factory* m_factory1D;
};