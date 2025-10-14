#pragma once

#include <array>
#include <compare>
#include <ostream>
#include <string>

class HierLevelIndex
{
public:
   HierLevelIndex(size_t level, size_t index);
   size_t getLevel() const;
   size_t getIndex() const;

   double toDouble() const;

   std::array<HierLevelIndex, 2> refine() const;

   auto operator<=>(const HierLevelIndex&) const = default;

   friend std::ostream& operator<<(std::ostream& os, const HierLevelIndex& hindex)
   {
      return os << "{" << hindex.getLevel() << ", " << hindex.getIndex() << "}";
   }
private:
   size_t m_level;
   size_t m_index;
};
