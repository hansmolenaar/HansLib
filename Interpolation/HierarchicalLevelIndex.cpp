#include "HierarchicalLevelIndex.h"
#include "Utilities/MyException.h"
#include "Utilities/Pow2.h"

namespace
{

   void CheckLevelIndex(size_t level, size_t index)
   {
      if (level == 0)
      {
         if (index > 1) throw MyException("HierarchicalMultiIndex CheckIndex level=" + std::to_string(level) + " index=" + std::to_string(index));
         return;
      }

      if (index > Pow2()(level)) throw MyException("HierarchicalMultiIndex CheckIndex level=" + std::to_string(level) + " index=" + std::to_string(index));
   }
}

HierarchicalLevelIndex::HierarchicalLevelIndex(size_t level, size_t index) : m_level(level), m_index(index)
{
   CheckLevelIndex(m_level, m_index);
}

size_t HierarchicalLevelIndex::getLevel() const
{
   return m_level;
}


size_t HierarchicalLevelIndex::getIndex() const
{
   return m_index;
}