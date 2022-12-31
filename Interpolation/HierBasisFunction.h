#pragma once

#include "IHierBasisFunction.h"

class IHierBasisFunction1D;

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
