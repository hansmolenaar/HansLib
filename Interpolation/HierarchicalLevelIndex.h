#pragma once

#include <compare>

class HierarchicalLevelIndex
{
public:
   HierarchicalLevelIndex(size_t level, size_t index);
   size_t getLevel() const;
   size_t getIndex() const;

   auto operator<=>(const HierarchicalLevelIndex&) const = default;
private:
   size_t m_level;
   size_t m_index;
};
