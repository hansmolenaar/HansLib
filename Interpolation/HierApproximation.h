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
   using GetRefinements = std::function<std::vector<RefinementSpecification>(const HierApproximation&)>;

   struct GetRefineInAllDirectionsOnRefinementLevel
   {
      int MaxRefinementLevel = std::numeric_limits<int>::max();
      std::vector<RefinementSpecification> operator()(const HierApproximation& approx) const
      {
         std::vector<RefinementSpecification> result;
         const auto& factory = approx.getFactory();
         for (const auto* leaf : approx.getLeafNodesRO())
         {
            const auto& mi = leaf->getMultiIndex();
            if (!factory.canBeRefined(mi)) continue;
            if (leaf->RefinementLevel < MaxRefinementLevel)
            {

               for (size_t d = 0; d < mi.getDimension(); ++d)
               {
                  result.emplace_back(mi, d);
               }
            }
         }
         return result;
      }
   };

   static std::unique_ptr<HierApproximation> Create(const IMultiVariableRealValuedFunction& fie, const IHierBasisFunction_Factory& factory, const GetRefinements& getRefinements);

   double operator()(std::span<const double>) const;

   std::vector<const HierTreeNode*> getLeafNodesRO() const;
   std::vector< HierTreeNode*> getLeafNodes() const;
   std::vector<const HierTreeNode*> getAllTreeNodesRO() const;

   std::vector<std::vector<double>> getCollocationPoints() const;
   double getMaxSurplus() const;
   const IHierBasisFunction_Factory& getFactory() const;

private:
   explicit HierApproximation(const IHierBasisFunction_Factory&);


   std::vector< HierTreeNode*> getAllTreeNodes() const;

   std::map<HierMultiIndex, std::unique_ptr<HierTreeNode>> m_treeNodeMap;
   std::vector<HierTreeNode*> m_root;;
   const IHierBasisFunction_Factory& m_factory;
   MultiIndex<size_t> m_indexer;
};