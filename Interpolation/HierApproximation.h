#pragma once

#include "IHierBasisFunction.h"
#include "IMultiVariableRealValuedFunction.h"
#include "HierMultiIndex.h"
#include "MultiIndex.h"
#include "Iota.h"
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
   size_t getLevel() const { return getMultiIndex().getL1NormLevel(); }
};



class HierApproximation // : public IMultiVariableRealValuedFunction
{
public:

   using RefinementSpecification = std::pair<HierMultiIndex, size_t>;

   static std::unique_ptr<HierApproximation> Create(const IMultiVariableFunctionEvaluate& fie, IHierBasisFunction_Factory& factory, INodeRefinePredicateFactory& refinementFactory);

   double operator()(std::span<const double>) const;

   std::vector<const HierTreeNode*> iterate(INodeRefinePredicateFactory& refinementFactory);

   std::vector<const HierTreeNode*> getLeafNodesRO() const;
   std::vector< HierTreeNode*> getLeafNodes() const;
   std::vector<const HierTreeNode*> getAllTreeNodesRO() const;

   int getMaxLevel() const;

   std::vector<std::vector<double>> getCollocationPoints() const;
   double getMaxSurplus() const;
   const IHierBasisFunction_Factory& getFactory() const;

private:
   HierApproximation(IHierBasisFunction_Factory&, const IMultiVariableFunctionEvaluate&);


   std::vector< HierTreeNode*> getAllTreeNodes() const;

   const IMultiVariableFunctionEvaluate& m_function;
   std::map<HierMultiIndex, std::unique_ptr<HierTreeNode>> m_treeNodeMap;
   std::vector<HierTreeNode*> m_root;
   IHierBasisFunction_Factory& m_factory;
   MultiIndex<size_t> m_indexer;
};