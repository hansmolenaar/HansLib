#pragma once

#include "HierLevelIndex.h"

#include <vector>

class HierMultiIndex
{
public:
   explicit HierMultiIndex(HierLevelIndex levelIndex);
   explicit HierMultiIndex(std::vector<HierLevelIndex>&& levelsIndices);
   size_t getDimension() const;
   const std::vector<HierLevelIndex>& get() const;
   std::vector< double> toDoubles() const;

   friend  std::strong_ordering operator<=>(const HierMultiIndex&, const HierMultiIndex&);
   bool operator==(const HierMultiIndex&) const = default;

   size_t getL1NormLevel() const;
   size_t getSupNormLevel() const;

private:
   std::vector<HierLevelIndex> m_levelsIndices;
};
