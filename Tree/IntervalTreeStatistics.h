#pragma once

#include "IntervalTree.h"
#include "IntervalTreeAction.h"
#include "StreamUtils.h"
#include <map>
#include <ostream>
#include <unordered_set>

namespace IntervalTree
{
   struct Statistics
   {
      int Size = -1;
      std::vector<int> NumberOfLeavesPerLevel;
      auto operator<=>(const Statistics&) const = default;

      friend std::ostream& operator<<(std::ostream& os, const Statistics& stats)
      {
         os << stats.Size;
         return StreamUtils::insertList(os, stats.NumberOfLeavesPerLevel, ", {", ", ", "}");
      }

   };

   template<int N>
   Statistics GetStatistics(const IndexTree<N>& tree)
   {
      Statistics result;

      ActionCountPerLevel<N> countPerLevel;
      tree.foreachLeaf(countPerLevel);
      result.Size = static_cast<int>(tree.size());

      result.NumberOfLeavesPerLevel = std::vector<int>(countPerLevel.Count.rbegin()->first + 1, 0);
      for (const auto& itr : countPerLevel.Count)
      {
         result.NumberOfLeavesPerLevel.at(itr.first) = itr.second;
      }

      return result;
   }

}
