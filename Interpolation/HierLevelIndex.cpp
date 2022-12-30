#include "HierLevelIndex.h"
#include "Utilities/MyException.h"
#include "Utilities/Pow2.h"
#include "Utilities/Assert.h"

namespace
{

   void CheckLevelIndex(const HierLevelIndex& li)
   {
      const auto level = li.getLevel();
      const auto index = li.getIndex();
      if (level == 0)
      {
         if (index > 1) throw MyException("HierarchicalMultiIndex CheckIndex at level=0 ??: " + li.toString());
         return;
      }

      if (index > Pow2()(level)) throw MyException("HierarchicalMultiIndex CheckIndex index out of bounds: " + li.toString());
      if (index % 2 == 0)throw MyException("HierarchicalMultiIndex CheckIndex index should be odd, actual=" + li.toString());
   }

   HierLevelIndex Reduce(size_t level, size_t index)
   {
      while (level > 0 && index % 2 == 0)
      {
         level -= 1;
         index /= 2;
      }
      return HierLevelIndex(level, index);
   }

}

HierLevelIndex::HierLevelIndex(size_t level, size_t index) : m_level(level), m_index(index)
{
   CheckLevelIndex(*this);
}

size_t HierLevelIndex::getLevel() const
{
   return m_level;
}


size_t HierLevelIndex::getIndex() const
{
   return m_index;
}


std::string HierLevelIndex::toString() const
{
   return "{" + std::to_string(getLevel()) + ", " + std::to_string(getIndex()) + "}";
}

double HierLevelIndex::toDouble() const
{
   return  static_cast<double>(getIndex()) / Pow2()(getLevel());
}

std::array<HierLevelIndex, 2> HierLevelIndex::refine() const
{
   return { HierLevelIndex(getLevel() + 1, 2 * getIndex() - 1), HierLevelIndex(getLevel() + 1, 2 * getIndex() + 1) };
}