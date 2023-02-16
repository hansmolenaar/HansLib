#pragma once

#include "IHierBasisFunction1D.h"
#include "HierLevelIndex.h"

#include <map>

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
   const IHierBasisFunction1D* get(const HierLevelIndex& li) override;
   std::vector<HierLevelIndex> getLowestLevel() const override;
   bool canBeRefined(const HierLevelIndex&) const override;
private:
   std::unique_ptr<HierBasisFunction1D_ExtraplolateBC> create(const HierLevelIndex& li) const;
   std::map<HierLevelIndex, std::unique_ptr<HierBasisFunction1D_ExtraplolateBC>> m_basisFunctions;
};