#pragma once

#include "HierarchicalLevelIndex.h"

#include <vector>

class HierarchicalMultiIndex
{
public:
   explicit HierarchicalMultiIndex(HierarchicalLevelIndex levelIndex);
   explicit HierarchicalMultiIndex(std::vector<HierarchicalLevelIndex>&& levelsIndices);
   size_t getDimension() const;
   const std::vector<HierarchicalLevelIndex>& get() const;

   auto operator<=>(const HierarchicalMultiIndex&) const = default;

   size_t getL1NormLevel() const;
   size_t getSupNormLevel() const;

private:
   std::vector<HierarchicalLevelIndex> m_levelsIndices;
};
