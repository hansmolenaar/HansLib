#pragma once

#include "HierLevelIndex.h"
#include "IHierBasisFunction1D.h"
#include "Functions/ISingleVariableRealValuedFunction.h"
#include "HierMultiIndex.h"
#include "HierRefinementInfo.h"

#include <memory>
#include <functional>
#include <map>


struct HierTreeNode1D
{
   double operator()(double) const;

   const IHierBasisFunction1D* BasisFunction;
   double Surplus;
   std::vector<HierTreeNode1D*> Kids;
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

   double getMaxSurplus() const;

private:
   explicit HierApproximation1D(const IHierBasisFunction1D_Factory&);
   std::vector< HierTreeNode1D*> getAllTreeNodes() const;
   std::vector< HierTreeNode1D*> getLeafNodes() const;

   std::map<HierMultiIndex, std::unique_ptr<HierTreeNode1D>> m_treeNodeMap;
   std::vector<HierTreeNode1D*> m_root;
   const IHierBasisFunction1D_Factory& m_factory;
};