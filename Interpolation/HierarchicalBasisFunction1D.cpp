#include "HierarchicalBasisFunction1D.h"

#include "Utilities/MyException.h"

#include <cmath>


namespace
{
   size_t GetPow2(size_t n)
   {
      return static_cast<size_t>(1) << n;
   }
}

void HierarchicalBasisFunction1D::CheckLevelIndex(size_t level, size_t index)
{
   if (level == 0)
   {
      if (index > 1) throw MyException("HierarchicalMultiIndex CheckIndex level=" + std::to_string(level) + " index=" + std::to_string(index));
      return;
   }

   if (index > GetPow2(level)) throw MyException("HierarchicalMultiIndex CheckIndex level=" + std::to_string(level) + " index=" + std::to_string(index));
}

HierarchicalBasisFunction1D::HierarchicalBasisFunction1D(size_t level, size_t index) :
   m_level(level), m_index(index)
{
   CheckLevelIndex(level, index);
}

double HierarchicalBasisFunction1D::evaluate(double x) const
{
   if (x < 0.0) return 0;
   if (x > 1.0) return 0;

   // Level 0 is different
   if (m_level == 0)
   {
      const double h = 0.5;
      if (m_index == 0)
      {
         return std::max(1.0 - x / h, 0.0);
      }
      else
      {
         return std::max(x / h - 1.0, 0.0);
      }
   }

   const double h = 1.0 / GetPow2(m_level);
   if (x < (m_index - 1.0) * h) return 0.0;
   if (x > (m_index + 1.0) * h) return 0.0;

   return 1.0 - std::abs(x / h - m_index);
}