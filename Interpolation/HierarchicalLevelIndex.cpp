#include "HierarchicalLevelIndex.h"
#include "Utilities/MyException.h"
#include "Utilities/Pow2.h"

namespace
{

   void CheckLevelIndex(const HierarchicalLevelIndex& li)
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

   HierarchicalLevelIndex Reduce(size_t level, size_t index)
   {
      while (level > 0 && index % 2 == 0)
      {
         level -= 1;
         index /= 2;
      }
      return HierarchicalLevelIndex(level, index);
   }

}

HierarchicalLevelIndex::HierarchicalLevelIndex(size_t level, size_t index) : m_level(level), m_index(index)
{
   CheckLevelIndex(*this);
}

size_t HierarchicalLevelIndex::getLevel() const
{
   return m_level;
}


size_t HierarchicalLevelIndex::getIndex() const
{
   return m_index;
}

HierarchicalLevelIndex HierarchicalLevelIndex::left() const
{
   if (getLevel() == 0) throw MyException("No neighbors at level 0: " + this->toString());
   return Reduce( getLevel() - 1, (getIndex() - 1) / 2 );
}

HierarchicalLevelIndex HierarchicalLevelIndex::right() const
{
   if (getLevel() == 0) throw MyException("No neighbors at level 0: " + this->toString());
   return Reduce( getLevel() - 1, (getIndex() + 1) / 2 );
}

std::string HierarchicalLevelIndex::toString() const
{
   return "{" + std::to_string(getLevel()) + ", " + std::to_string(getIndex()) + "}";
}