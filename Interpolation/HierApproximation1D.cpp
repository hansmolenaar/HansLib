#include "HierApproximation1D.h"
#include <numeric>

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
      result += r(x);
   }
   return result;
}

std::unique_ptr<HierApproximation1D> HierApproximation1D::Create(
   const ISingleVariableRealValuedFunction& fie, const IHierBasisFunction1D_Factory& factory, const std::function<bool(const HierRefinementInfo&)>& doRefine)
{
   std::unique_ptr<HierApproximation1D> result(new HierApproximation1D);

   for (auto b : factory.getLowestLevel())
   {
      result->m_root.emplace_back(HierTreeNode{ factory.create(b), 0.0 });
      // Calculate the surplus
      const double x = result->m_root.back().BasisFunction->getLevelIndex().toDouble();
      const double functionEval = fie.Evaluate(x);
      // Basis function may overlap
      const double approx = (*result)(x);
      result->m_root.back().Surplus = functionEval - approx;
   }

   return result;
}