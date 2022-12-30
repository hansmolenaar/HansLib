#include "HierApproximation1D.h"
#include "Utilities/Single.h"
#include "Utilities/Defines.h"

namespace
{
   void GetAllTreeNodesRecur(const std::vector<std::shared_ptr<HierTreeNode>>& treeNodes, std::vector<const HierTreeNode*>& result)
   {
      str::transform(treeNodes, std::back_inserter(result), [](const auto& sharedPointer) {return sharedPointer.get(); });
      for (const auto& tn : treeNodes)
      {
         GetAllTreeNodesRecur(tn->Kids, result);
      }
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

   for (auto b : factory.getLowestLevel())
   {
      result->m_root.emplace_back(std::make_shared<HierTreeNode>(HierTreeNode{ factory.create(b), 0.0 }));
      // Calculate the surplus
      const double x = result->m_root.back()->BasisFunction->getLevelIndex().toDouble();
      const double functionEval = fie.Evaluate(x);
      // Basis function may overlap
      const double approx = (*result)(x);
      result->m_root.back()->Surplus = functionEval - approx;
   }

   std::vector<HierTreeNode* > active;
   for (const auto& k : result->m_root)
   {
      const auto li = k->BasisFunction->getLevelIndex();
      if (factory.canBeRefined(li))
      {
         active.emplace_back(k.get());
      }
   }

   while (!active.empty())
   {
      const std::vector< HierTreeNode*> toRefine(active.begin(), active.end());
      active.clear();

      for (auto ref : toRefine)
      {
         const auto refLevelIndex = ref->BasisFunction->getLevelIndex();
         if (refLevelIndex.getLevel() <= 1)
         {
            for (const auto& k : refLevelIndex.refine())
            {
               auto kid = std::make_shared<HierTreeNode>(factory.create(k), 0.0);

               // Calculate the surplus
               const double x = k.toDouble();
               const double functionEval = fie.Evaluate(x);
               // Basis function may overlap
               const double approx = (*result)(x);
               kid->Surplus = functionEval - approx;
               ref->Kids.emplace_back(std::move(kid));
            }
         }
      }
   }

   return result;
}

std::vector<const HierTreeNode*> HierApproximation1D::getAllTreeNodes() const
{
   std::vector<const HierTreeNode*> result;
   GetAllTreeNodesRecur(m_root, result);
   return result;
}

std::vector<std::vector<double>> HierApproximation1D::getCollocationPoints() const
{
   const std::vector<const HierTreeNode*> allTreeNodes = getAllTreeNodes();
   std::vector<std::vector<double>> result(allTreeNodes.size());
   str::transform(allTreeNodes, result.begin(), [](const HierTreeNode* tn) {return std::vector<double>{ tn->BasisFunction->getLevelIndex().toDouble()}; });
   return result;
}