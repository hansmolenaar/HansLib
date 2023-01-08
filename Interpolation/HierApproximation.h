#pragma once

#include "IHierBasisFunction.h"
#include "Functions/IMultiVariableRealValuedFunction.h"
#include "HierMultiIndex.h"
#include "HierRefinementInfo.h"
#include "Utilities/MultiIndex.h"

#include <memory>
#include <functional>
#include <map>


struct HierTreeNode
{
   double operator()(std::span<const double>) const;

   const IHierBasisFunction* BasisFunction;
   double Surplus = 0.0;
   std::vector<HierTreeNode*> Kids;
};



class HierApproximation // : public IMultiVariableRealValuedFunction
{
public:
   static std::unique_ptr<HierApproximation> Create(const HierApproximation& fie, const IHierBasisFunction_Factory& factory, const std::function<bool(const HierRefinementInfo&)>& doRefine);

   double operator()(std::span<const double>) const;
private:
   explicit HierApproximation(const IHierBasisFunction_Factory&);

   std::vector< HierTreeNode*> getAllTreeNodes() const;

   std::map<HierMultiIndex, std::unique_ptr<HierTreeNode>> m_treeNodeMap;
   std::vector<HierTreeNode*> m_root;
   const IHierBasisFunction_Factory& m_factory;
   MultiIndex<size_t> m_indexer;
};