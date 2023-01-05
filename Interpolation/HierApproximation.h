#pragma once

#include "IHierBasisFunction.h"
#include "Functions/IMultiVariableRealValuedFunction.h"
#include "HierMultiIndex.h"
#include "HierRefinementInfo.h"

#include <memory>
#include <functional>
#include <map>


struct HierTreeNode
{
   double operator()(std::span<const double>) const;

   const IHierBasisFunction* BasisFunction;
   double Surplus;
   std::vector<HierTreeNode*> Kids;
};



class HierApproximation : public IMultiVariableRealValuedFunction
{
public:
private:
   explicit HierApproximation(const IHierBasisFunction_Factory&);

   std::map<HierMultiIndex, std::unique_ptr<HierTreeNode>> m_treeNodeMap;
   std::vector<HierTreeNode*> m_root;
   const IHierBasisFunction_Factory& m_factory;
};