#pragma once

#include <compare>
#include <string>

class HierarchicalLevelIndex
{
public:
   HierarchicalLevelIndex(size_t level, size_t index);
   size_t getLevel() const;
   size_t getIndex() const;

   HierarchicalLevelIndex left() const;
   HierarchicalLevelIndex right() const;

   std::string toString() const;
   double toDouble() const;

   auto operator<=>(const HierarchicalLevelIndex&) const = default;
private:
   size_t m_level;
   size_t m_index;
};
