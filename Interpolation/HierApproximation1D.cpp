#include "HierApproximation1D.h"
#include "Utilities/Single.h"
#include "Utilities/Defines.h"

namespace
{

   struct IsLeaf
   {
      bool operator()(const HierTreeNode& htn) const
      {
         return htn.Kids.empty();
      }
   };


   struct DoRefine
   {
      const IHierBasisFunction1D_Factory& Factory;
      const std::function<bool(const HierRefinementInfo&)>& RefinementPredicate;

      bool operator()(const HierTreeNode* htn) const
      {
         const auto li = htn->BasisFunction->getLevelIndex();
         if ( !Factory.canBeRefined(li)) return false;
         const HierRefinementInfo refinementInfo{ li.getLevel() };
         return RefinementPredicate(refinementInfo);
      }
   };

   void GetAllTreeNodesRecur(const std::vector<std::shared_ptr<HierTreeNode>>& treeNodes, std::vector< HierTreeNode*>& result)
   {
      str::transform(treeNodes, std::back_inserter(result), [](const auto& sharedPointer) {return sharedPointer.get(); });
      for (const auto& tn : treeNodes)
      {
         GetAllTreeNodesRecur(tn->Kids, result);
      }
   }


   void GetLeafNodesRecur(const std::vector<std::shared_ptr<HierTreeNode>>& treeNodes, std::vector< HierTreeNode*>& result)
   {
      constexpr IsLeaf isLeaf;
      for (const auto& tn : treeNodes)
      {
         if (isLeaf(*tn))
         {
            result.push_back(tn.get());
         }
         else
         {
            GetLeafNodesRecur(tn->Kids, result);
         }
      }
   }

   std::shared_ptr<HierTreeNode> CreateKid(const IHierBasisFunction1D_Factory& factory, const HierLevelIndex& kidLevelIndex, const ISingleVariableRealValuedFunction& fie, const HierApproximation1D& approximation)
   {
      auto kid = std::make_shared<HierTreeNode>(factory.create(kidLevelIndex), 0.0);

      // Calculate the surplus
      const double x = kidLevelIndex.toDouble();
      const double functionEval = fie.Evaluate(x);
      // Basis function may overlap
      const double approx = approximation(x);
      kid->Surplus = functionEval - approx;
      return kid;
   }

}

double HierTreeNode::operator()(double x) const
{
   if (!BasisFunction->getSupport().contains(x)) return 0;
   double result = Surplus * (*BasisFunction)(x);;
   for (const auto& kid : Kids)
   {
      result += (*kid)(x);
   }
   return result;
}


double HierApproximation1D::operator()(double x) const
{
   double result = 0;
   for (const auto& r : m_root)
   {
      result += (*r)(x);
   }
   return result;
}

std::unique_ptr<HierApproximation1D> HierApproximation1D::Create(
   const ISingleVariableRealValuedFunction& fie, const IHierBasisFunction1D_Factory& factory, const std::function<bool(const HierRefinementInfo&)>& doRefine)
{
   std::unique_ptr<HierApproximation1D> result(new HierApproximation1D);
   const DoRefine refinementPredicate{factory, doRefine };

   for (auto b : factory.getLowestLevel())
   {
      result->m_root.emplace_back(CreateKid(factory, b, fie, *result));
   }

   const auto leafNodes = result->getLeafNodes();
   std::vector<HierTreeNode* > toRefine;
   str::copy_if(leafNodes, std::back_inserter(toRefine), refinementPredicate);

   while (!toRefine.empty())
   {
      for (auto ref : toRefine)
      {
         for (const auto& k : ref->BasisFunction->getLevelIndex().refine())
         {
            ref->Kids.emplace_back(CreateKid(factory, k, fie, *result));
         }
      }

      const auto leafNodes = result->getLeafNodes();
      toRefine.clear();
      str::copy_if(leafNodes, std::back_inserter(toRefine), refinementPredicate);
   }

   return result;
}

std::vector< HierTreeNode*> HierApproximation1D::getAllTreeNodes() const
{
   std::vector< HierTreeNode*> result;
   GetAllTreeNodesRecur(m_root, result);
   return result;
}

std::vector< HierTreeNode*> HierApproximation1D::getLeafNodes() const
{
   std::vector< HierTreeNode*> result;
   GetLeafNodesRecur(m_root, result);
   return result;
}

std::vector<std::vector<double>> HierApproximation1D::getCollocationPoints() const
{
   const std::vector< HierTreeNode*> allTreeNodes = getAllTreeNodes();
   std::vector<std::vector<double>> result(allTreeNodes.size());
   str::transform(allTreeNodes, result.begin(), [](const HierTreeNode* tn) {return std::vector<double>{ tn->BasisFunction->getLevelIndex().toDouble()}; });
   return result;
}