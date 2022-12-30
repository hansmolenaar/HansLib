#pragma once

#include <compare>
#include <string>
#include <array>

class HierLevelIndex
{
public:
   HierLevelIndex(size_t level, size_t index);
   size_t getLevel() const;
   size_t getIndex() const;

   std::string toString() const;
   double toDouble() const;

   std::array<HierLevelIndex, 2> refine() const;

   auto operator<=>(const HierLevelIndex&) const = default;
private:
   size_t m_level;
   size_t m_index;
};
