#pragma once

#include "IHierBasisFunction1D.h"
#include "HierLevelIndex.h"

class HierBasisFunction1D_ExtraplolateBC : public IHierBasisFunction1D
{
public:
   explicit HierBasisFunction1D_ExtraplolateBC(HierLevelIndex);
   double operator()(double x) const override;
   HierLevelIndex getLevelIndex() const override;
   Interval<double> getSupport() const override;

private:
   HierLevelIndex m_levelIndex;
};

class HierBasisFunction1D_ExtraplolateBC_Factory : public IHierBasisFunction1D_Factory
{
public:
   std::unique_ptr<IHierBasisFunction1D> create(const HierLevelIndex& li) const override;
   std::vector<HierLevelIndex> getLowestLevel() const override;
   bool canBeRefined(const HierLevelIndex&) const override;
};