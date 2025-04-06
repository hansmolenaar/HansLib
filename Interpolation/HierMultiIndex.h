#pragma once

#include "HierLevelIndex.h"

#include <array>
#include <functional>
#include <vector>

class HierMultiIndex
{
public:
   explicit HierMultiIndex(HierLevelIndex levelIndex);
   explicit HierMultiIndex(std::vector<HierLevelIndex>&& levelsIndices);
   size_t getDimension() const;
   const std::vector<HierLevelIndex>& get() const;
   std::vector< double> toDoubles() const;

   friend std::ostream& operator<<(std::ostream& os, const HierMultiIndex& hmi)
   {
      bool first = true;
      for (const auto& li : hmi.m_levelsIndices)
      {
         if (!first) os << " ";
         first = false;
         os << li;
      }
      return os;
   }


   std::array<HierMultiIndex, 2> refine(size_t dir) const;

   friend  std::strong_ordering operator<=>(const HierMultiIndex&, const HierMultiIndex&);
   bool operator==(const HierMultiIndex&) const = default;

   size_t getL1NormLevel() const;
   size_t getSupNormLevel() const;

private:
   HierMultiIndex() = default;
   std::vector<HierLevelIndex> m_levelsIndices;
};
