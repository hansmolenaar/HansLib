#pragma once

#include "IHierBasisFunction1D.h"
#include "HierLevelIndex.h"

class HierBasisFunction1D_HomogenousBC : public IHierBasisFunction1D
{
public:
   explicit HierBasisFunction1D_HomogenousBC(HierLevelIndex);
   double operator()(double x) const override;
   HierLevelIndex getLevelIndex() const override;
   Interval<double> getSupport() const override;
  
private:
   HierLevelIndex m_levelIndex;
};

class HierBasisFunction1D_HomogenousBC_Factory : public IHierBasisFunction1D_Factory
{
public:
   inline std::unique_ptr<IHierBasisFunction1D> create(const HierLevelIndex& li) const override { return std::make_unique<HierBasisFunction1D_HomogenousBC>(li); }
   inline std::vector<HierLevelIndex> getLowestLevel() const override { return std::vector<HierLevelIndex>{HierLevelIndex(1,1)}; }
   inline bool isBoundaryFunction(const HierLevelIndex&) const { return false; }
};