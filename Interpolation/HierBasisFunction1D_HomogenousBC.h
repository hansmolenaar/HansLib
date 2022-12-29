#pragma once

#include "IHierBasisFunction1D.h"
#include "HierLevelIndex.h"

class HierBasisFunction1D_HomogenousBC : public IHierBasisFunction1D
{
public:
   explicit HierBasisFunction1D_HomogenousBC(HierLevelIndex);
   double operator()(double x) const override;
  
private:
   HierLevelIndex m_levelIndex;
};

class HierBasisFunction1D_HomogenousBC_Factory : public IHierBasisFunction1D_Factory
{
public:
   inline bool hasLevel0() const override { return false; }
   inline std::unique_ptr<IHierBasisFunction1D> create(const HierLevelIndex& li) override { return std::make_unique<HierBasisFunction1D_HomogenousBC>(li); }
};