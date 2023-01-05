#include "HierApproximation.h"

HierApproximation::HierApproximation(const IHierBasisFunction_Factory& factory) : m_factory(factory)
{
}


double HierTreeNode::operator()(std::span<const double> x) const
{
   if (!BasisFunction->contains(x)) return 0;
   double result = Surplus * (*BasisFunction)(x);;
   for (const auto& kid : Kids)
   {
      result += (*kid)(x);
   }
   return result;
}