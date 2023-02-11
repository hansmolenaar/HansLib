#pragma once

#include "IHierBasisFunction.h"
#include "Functions/IMultiVariableRealValuedFunction.h"
#include "HierMultiIndex.h"
#include "HierRefinementInfo.h"
#include "Utilities/MultiIndex.h"
#include "Utilities/Iota.h"

#include <memory>
#include <functional>
#include <map>
#include <limits>


struct HierTreeNode
{
   double operator()(std::span<const double>) const;

   const IHierBasisFunction* BasisFunction;
   double Surplus = std::numeric_limits<double>::quiet_NaN();
   double Value = std::numeric_limits<double>::quiet_NaN();
   std::vector<HierTreeNode*> Kids;
   const HierMultiIndex& getMultiIndex() const { return BasisFunction->getMultiIndex(); }
};



class HierApproximation // : public IMultiVariableRealValuedFunction
{
public:
   using RefineInDirections = std::function<std::vector<size_t>(const HierRefinementInfo&)>;

   struct RefineInAllDirectionsOnL1Norm
   {
      size_t MaxL1Norm = std::numeric_limits<size_t>::max();
      std::vector<size_t> operator()(const HierRefinementInfo& hri) const
      {
         if (hri.MultiLevelIndex.getL1NormLevel() >= MaxL1Norm) return {};
         return Iota::GenerateVector<size_t>(hri.MultiLevelIndex.getDimension());
      }
   };
   static std::unique_ptr<HierApproximation> Create(const IMultiVariableRealValuedFunction& fie, const IHierBasisFunction_Factory& factory, const RefineInDirections& toRefine);

   double operator()(std::span<const double>) const;

   std::vector<const HierTreeNode*> getLeafNodesRO() const;
   std::vector< HierTreeNode*> getLeafNodes() const;
   std::vector<const HierTreeNode*> getAllTreeNodesRO() const;

   std::vector<std::vector<double>> getCollocationPoints() const;
   double getMaxSurplus() const;

private:
   explicit HierApproximation(const IHierBasisFunction_Factory&);


   std::vector< HierTreeNode*> getAllTreeNodes() const;

   std::map<HierMultiIndex, std::unique_ptr<HierTreeNode>> m_treeNodeMap;
   std::vector<HierTreeNode*> m_root;;
   const IHierBasisFunction_Factory& m_factory;
   MultiIndex<size_t> m_indexer;
};