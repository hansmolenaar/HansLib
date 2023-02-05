#pragma once

#include "HierLevelIndex.h"

#include <array>
#include <vector>
#include <functional>

class HierMultiIndex
{
public:
   explicit HierMultiIndex(HierLevelIndex levelIndex);
   explicit HierMultiIndex(std::vector<HierLevelIndex>&& levelsIndices);
   size_t getDimension() const;
   const std::vector<HierLevelIndex>& get() const;
   std::vector< double> toDoubles() const;

   using RefineInDirectionPredicate = std::function<bool(const HierMultiIndex&, size_t)>;
   struct RefineInAllDirections
   {
      bool operator()(const HierMultiIndex&, size_t) const { return true; }
   };

   std::vector<HierMultiIndex> refine(const RefineInDirectionPredicate&) const;
   std::array<HierMultiIndex, 2> refine(size_t dir) const;

   friend  std::strong_ordering operator<=>(const HierMultiIndex&, const HierMultiIndex&);
   bool operator==(const HierMultiIndex&) const = default;

   size_t getL1NormLevel() const;
   size_t getSupNormLevel() const;

private:
   HierMultiIndex() = default;
   std::vector<HierLevelIndex> m_levelsIndices;
};
