#pragma once

#include "IHierBasisFunction.h"
#include "Functions/IMultiVariableRealValuedFunction.h"
#include "HierMultiIndex.h"
#include "Utilities/MultiIndex.h"
#include "Utilities/Iota.h"
#include "INodeRefinePredicateFactory.h"

#include <memory>
#include <functional>
#include <map>
#include <limits>


struct HierTreeNode
{
   double operator()(std::span<const double>) const;

   const IHierBasisFunction* BasisFunction;
   int RefinementLevel = std::numeric_limits<int>::min();
   double Surplus = std::numeric_limits<double>::quiet_NaN();
   double Value = std::numeric_limits<double>::quiet_NaN();
   std::vector<HierTreeNode*> Kids;
   const HierMultiIndex& getMultiIndex() const { return BasisFunction->getMultiIndex(); }
};



class HierApproximation // : public IMultiVariableRealValuedFunction
{
public:

   using RefinementSpecification = std::pair<HierMultiIndex, size_t>;

   static std::unique_ptr<HierApproximation> Create(const IMultiVariableRealValuedFunction& fie, IHierBasisFunction_Factory& factory, INodeRefinePredicateFactory& refinementFactory);

   double operator()(std::span<const double>) const;

   std::vector<const HierTreeNode*> getLeafNodesRO() const;
   std::vector< HierTreeNode*> getLeafNodes() const;
   std::vector<const HierTreeNode*> getAllTreeNodesRO() const;

   std::vector<std::vector<double>> getCollocationPoints() const;
   double getMaxSurplus() const;
   const IHierBasisFunction_Factory& getFactory() const;

private:
   explicit HierApproximation(IHierBasisFunction_Factory&);


   std::vector< HierTreeNode*> getAllTreeNodes() const;

   std::map<HierMultiIndex, std::unique_ptr<HierTreeNode>> m_treeNodeMap;
   std::vector<HierTreeNode*> m_root;;
   IHierBasisFunction_Factory& m_factory;
   MultiIndex<size_t> m_indexer;
};