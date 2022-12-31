#pragma once

#include "IHierBasisFunction1D.h"
#include "HierLevelIndex.h"

class HierBasisFunction1D_ExtendedLevelOneBC : public IHierBasisFunction1D
{
public:
   explicit HierBasisFunction1D_ExtendedLevelOneBC(HierLevelIndex);
   double operator()(double x) const override;
   HierLevelIndex getLevelIndex() const override;
   Interval<double> getSupport() const override;

private:
   HierLevelIndex m_levelIndex;
};

class HierBasisFunction1D_ExtendedLevelOneBC_Factory : public IHierBasisFunction1D_Factory
{
public:
   const IHierBasisFunction1D* get(const HierLevelIndex& li) const override;
   std::vector<HierLevelIndex> getLowestLevel() const override;
   bool canBeRefined(const HierLevelIndex&) const override;
private:
   std::unique_ptr<HierBasisFunction1D_ExtendedLevelOneBC> create(const HierLevelIndex& li) const;
};