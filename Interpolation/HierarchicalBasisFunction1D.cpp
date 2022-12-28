#include "HierarchicalBasisFunction1D.h"

#include "Utilities/MyException.h"
#include "Utilities/Pow2.h"

#include <cmath>

HierarchicalBasisFunction1D::HierarchicalBasisFunction1D(HierarchicalLevelIndex li)  : m_levelIndex(li)
{
}

double HierarchicalBasisFunction1D::operator()(double x) const
{
   if (x < 0.0) return 0;
   if (x > 1.0) return 0;

   const auto level = m_levelIndex.getLevel();
   const auto index = m_levelIndex.getIndex();

   // Level 0 is different
   if (level == 0)
   {
      const double h = 0.5;
      if (index == 0)
      {
         return std::max(1.0 - x / h, 0.0);
      }
      else
      {
         return std::max(x / h - 1.0, 0.0);
      }
   }

   const double h = 1.0 / Pow2()(level);
   if (x < (index - 1.0) * h) return 0.0;
   if (x > (index + 1.0) * h) return 0.0;

   return 1.0 - std::abs(x / h - index);
}