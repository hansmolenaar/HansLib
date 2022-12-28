#include "HierarchicalApproximation1D.h"

#include "Utilities//Defines.h"
#include  "Functions/ISingleVariableRealValuedFunction.h"
#include  "Utilities/Pow2.h"

#include <set>
#include <limits>

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

   // Recursion only possible from first level
   const HierarchicalLevelIndex mid(1,1);
   const double fieM = fie.Evaluate(std::array<double, 1>{mid.toDouble()});
   result->m_functions.emplace(mid, ValueSurplus{ fieM, fieM });

   std::set<HierarchicalLevelIndex> active;
   for (const auto& nxt : mid.next())
   {
      active.insert(nxt);
   }
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
            const auto checkIsNew = result->m_functions.emplace(ref, ValueSurplus{ value, surplus });
            Utilities::Assert(checkIsNew.second);

            for (const auto& nxt : ref.next())
            {
               active.insert(nxt);
            }
         }
      }
   }

   return result;
}

bool HierarchicalApproximation1D::isLeaf(const HierarchicalLevelIndex& li) const
{
   Utilities::Assert(m_functions.contains(li));
   if (li.getLevel() == 0) return false;
   const auto kids = li.next();
   return !m_functions.contains(kids[0]) && !m_functions.contains(kids[1]);
}

double HierarchicalApproximation1D::getMaxSurplus() const
{
   double result = 0;
   for (const auto& itr : m_functions)
   {
      if (isLeaf(itr.first))
      {
         result = std::max(result, std::abs(itr.second.Surplus));
      }
   }
   return result;
}

size_t HierarchicalApproximation1D::numLeaves() const
{
   return str::count_if(m_functions, [this](const auto& itr) {return isLeaf(itr.first); });
}