#pragma once

#include "HierLevelIndex.h"
#include "IHierBasisFunction1D.h"

#include <memory>
#include <functional>

class ISingleVariableRealValuedFunction;

struct ValueSurplus
{
   double Value;
   double Surplus;
};

struct HierRefinementInfo
{
   size_t Level;
};

struct HierTreeNode
{
   double operator()(double) const;

   std::unique_ptr<IHierBasisFunction1D> BasisFunction;
   double Surplus;
   std::vector<std::shared_ptr<HierTreeNode>> Kids;
};

class HierApproximation1D
{
public:
   static std::unique_ptr<HierApproximation1D> Create(const ISingleVariableRealValuedFunction& fie, const IHierBasisFunction1D_Factory& factory, const std::function<bool(const HierRefinementInfo&)>& doRefine);
   double operator()(double) const;
   //bool isLeaf(const HierLevelIndex&) const;
   //double getMaxSurplus() const;
   //size_t numLeaves() const;
private:
   HierApproximation1D() = default;
   std::vector<std::shared_ptr<HierTreeNode>> m_root;
};