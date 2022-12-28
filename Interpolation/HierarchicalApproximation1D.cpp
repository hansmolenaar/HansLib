#include "HierarchicalApproximation1D.h"

#include  "Functions/ISingleVariableRealValuedFunction.h"
#include  "Utilities/Pow2.h"
#include <numeric>
#include <array>
#include <set>

size_t HierarchicalApproximation1D::getDimension() const
{
   return 1;
}

double HierarchicalApproximation1D::operator()(std::span<const double> x) const
{
   return std::accumulate(m_functions.begin(), m_functions.end(), 0.0, [&x](double acc, const auto& index_coef) {return acc + index_coef.second.Surplus * HierarchicalBasisFunction(HierarchicalMultiIndex(index_coef.first))(x); });
}

std::unique_ptr<HierarchicalApproximation1D> HierarchicalApproximation1D::CreateToLevel(const ISingleVariableRealValuedFunction& fie, size_t level)
{
   std::unique_ptr<HierarchicalApproximation1D> result(new HierarchicalApproximation1D());
   const double fie0 = fie.Evaluate(std::array<double, 1>{0.0});
   const double fie1 = fie.Evaluate(std::array<double, 1>{1.0});
   result->m_functions.emplace(HierarchicalLevelIndex(0, 0), ValueSurplus{ fie0, fie0 });
   result->m_functions.emplace(HierarchicalLevelIndex(0, 1), ValueSurplus{ fie1, fie1 });

   if (level == 0) return result;

   const double fieM = fie.Evaluate(std::array<double, 1>{0.5});
   result->m_functions.emplace(HierarchicalLevelIndex(1, 1), ValueSurplus{ fieM, fieM });

   std::set<HierarchicalLevelIndex> active;
   active.insert(HierarchicalLevelIndex(1, 1));
   while (!active.empty())
   {
      std::vector< HierarchicalLevelIndex> todo(active.begin(), active.end());
      active.clear();

      for (auto ref : todo)
      {
         if (ref.getLevel() <= level)
         {
            const auto left = ref.left();
            const auto right = ref.right();
            const double leftValue = result->m_functions.at(left).Value;
            const double rightValue = result->m_functions.at(right).Value;
            const double value = fie.Evaluate(std::array<double, 1>{ref.toDouble()});
            const double surplus = value - 0.5 * (leftValue + rightValue);
            result->m_functions.emplace(ref, ValueSurplus{ value, surplus });

            active.insert(HierarchicalLevelIndex(ref.getLevel() + 1, 2 * ref.getIndex() - 1));
            active.insert(HierarchicalLevelIndex(ref.getLevel() + 1, 2 * ref.getIndex() + 1));
         }
      }
   }

   return result;
}