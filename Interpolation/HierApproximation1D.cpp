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

      bool operator()(const HierTreeNode* htn) const
      {
         return (*this)(*htn);
      }
   };

   struct IsRefinable
   {
      const IHierBasisFunction1D_Factory& Factory;

      bool operator()(const HierTreeNode* htn) const
      {
         const auto li = htn->BasisFunction->getLevelIndex();
         return Factory.canBeRefined(li);
      }
   };

   struct DoRefine
   {
      const IHierBasisFunction1D_Factory& Factory;
      const std::function<bool(const HierRefinementInfo&)>& RefinementPredicate;
      double MaxSurplus = 0;

      bool operator()(const HierTreeNode* htn) const
      {
         if (!IsRefinable{ Factory }(htn)) return false;
         const auto li = htn->BasisFunction->getLevelIndex();
         const HierRefinementInfo refinementInfo{ li, std::abs(htn->Surplus), MaxSurplus };
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

   struct CreateKid
   {
      const IHierBasisFunction1D_Factory& Factory;
      const ISingleVariableRealValuedFunction& FunctionToApproximate;
      const HierApproximation1D& Approximation;

      std::shared_ptr<HierTreeNode> operator()(const HierLevelIndex& kidLevelIndex) const
      {
         auto kid = std::make_shared<HierTreeNode>(Factory.get(kidLevelIndex), 0.0);

         // Calculate the surplus
         const double x = kidLevelIndex.toDouble();
         const double functionEval = FunctionToApproximate.Evaluate(x);
         // Basis function may overlap
         const double approx = Approximation(x);
         kid->Surplus = functionEval - approx;
         return kid;
      }
   };

}

HierApproximation1D::HierApproximation1D(const IHierBasisFunction1D_Factory& factory) : m_factory(factory)
{
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

// Implementaton ISingleVariableRealValuedFunction

double HierApproximation1D::Evaluate(double x)const
{
   return (*this)(x);
}

double HierApproximation1D::Derivative(double x)const
{
   throw MyException("HierApproximation1D::Derivative not differentiable");
}

bool HierApproximation1D::IsNonConstant() const
{
   return true;
}

bool HierApproximation1D::HasDerivative() const
{
   return false;
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
   std::unique_ptr<HierApproximation1D> result(new HierApproximation1D(factory));
   DoRefine refinementPredicate{ factory, doRefine };
   const CreateKid createKid(factory, fie, *result);

   str::transform(factory.getLowestLevel(), std::back_inserter(result->m_root), createKid);

   std::vector<HierTreeNode*> toRefine;
   str::copy_if(result->getLeafNodes(), std::back_inserter(toRefine), refinementPredicate);

   while (!toRefine.empty())
   {
      for (auto ref : toRefine)
      {
         str::transform(ref->BasisFunction->getLevelIndex().refine(), std::back_inserter(ref->Kids), createKid);
      }

      toRefine.clear();
      refinementPredicate.MaxSurplus = result->getMaxSurplus();
      str::copy_if(result->getLeafNodes(), std::back_inserter(toRefine), refinementPredicate);
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
   str::sort(result);
   return result;
}

double HierApproximation1D::getMaxSurplus() const
{
   std::vector< HierTreeNode*> active;
   str::copy_if(getLeafNodes(), std::back_inserter(active), IsRefinable{ m_factory });
   return str::max(active | stv::transform([](const HierTreeNode* leaf) {return std::abs(leaf->Surplus); }));
}