#include "HierarchicalApproximation1D.h"

#include  "Functions/ISingleVariableRealValuedFunction.h"
#include <numeric>

size_t HierarchicalApproximation1D::getDimension() const
{
   return m_functions[0].getDimension();
}

double HierarchicalApproximation1D::operator()(std::span<const double> x) const
{
   return std::accumulate(m_functions.begin(), m_functions.end(), 0.0, [&x](double acc, const auto& fie) {return acc + fie(x); });
}

std::unique_ptr<HierarchicalApproximation1D> HierarchicalApproximation1D::CreateToLevel(const ISingleVariableRealValuedFunction& fie, size_t level)
{
   std::unique_ptr<HierarchicalApproximation1D> result(new HierarchicalApproximation1D());
   const double fie0 = fie.Evaluate(std::array<double, 1>{0.0});
   const double fie1 = fie.Evaluate(std::array<double, 1>{1.0});
   result->m_functions.push_back(HierarchicalApproximationFunction(HierarchicalMultiIndex({ 0, 0 }), fie0));
   result->m_functions.push_back(HierarchicalApproximationFunction(HierarchicalMultiIndex({ 0, 1 }), fie1));
   if (level == 0) return result;

   const double fieMid = fie.Evaluate(std::array<double, 1>{0.5});
   result->m_functions.push_back(HierarchicalApproximationFunction(HierarchicalMultiIndex({ 1, 1 }), fieMid));
   Utilities::Assert(level < 2);
   return result;
}