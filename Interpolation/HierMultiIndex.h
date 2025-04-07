#pragma once

#include "HierLevelIndex.h"
#include "StreamUtils.h"

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
      return StreamUtils::insertList(os, hmi.m_levelsIndices, "", " ", "");
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
