#pragma once

#include "HierLevelIndex.h"
#include "IHierBasisFunction1D.h"
#include "Functions/ISingleVariableRealValuedFunction.h"

#include <memory>
#include <functional>

struct ValueSurplus
{
   double Value;
   double Surplus;
};

struct HierRefinementInfo
{
   HierLevelIndex LevelIndex;
   double Surplus;
};

struct HierTreeNode
{
   double operator()(double) const;

   std::unique_ptr<IHierBasisFunction1D> BasisFunction;
   double Surplus;
   std::vector<std::shared_ptr<HierTreeNode>> Kids;
};

class HierApproximation1D : public ISingleVariableRealValuedFunction
{
public:
   static std::unique_ptr<HierApproximation1D> Create(const ISingleVariableRealValuedFunction& fie, const IHierBasisFunction1D_Factory& factory, const std::function<bool(const HierRefinementInfo&)>& doRefine);
   double operator()(double) const;
   std::vector<std::vector<double>> getCollocationPoints() const;

   double Evaluate(double x)const override;
   double Derivative(double x)const override;
   bool IsNonConstant() const override;
   bool HasDerivative() const override;


   //bool isLeaf(const HierLevelIndex&) const;
   double getMaxSurplus() const;
   //size_t numLeaves() const;
private:
   explicit HierApproximation1D(const IHierBasisFunction1D_Factory&);
   std::vector< HierTreeNode*> getAllTreeNodes() const;
   std::vector< HierTreeNode*> getLeafNodes() const;

   std::vector<std::shared_ptr<HierTreeNode>> m_root;
   const IHierBasisFunction1D_Factory& m_factory;
};