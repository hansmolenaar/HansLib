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
   static double Evaluate(const HierLevelIndex&, double);
   static Interval<double> GetSupport(const HierLevelIndex&);

private:
   HierLevelIndex m_levelIndex;
};

class HierBasisFunction1D_HomogenousBC_Factory : public IHierBasisFunction1D_Factory
{
public:
   const IHierBasisFunction1D* get(const HierLevelIndex& li) override;
   inline std::vector<HierLevelIndex> getLowestLevel() const override { return std::vector<HierLevelIndex>{HierLevelIndex(1, 1)}; }
   inline bool canBeRefined(const HierLevelIndex&) const { return true; }
private:
   inline std::unique_ptr<HierBasisFunction1D_HomogenousBC> create(const HierLevelIndex& li) const { return std::make_unique<HierBasisFunction1D_HomogenousBC>(li); }
};