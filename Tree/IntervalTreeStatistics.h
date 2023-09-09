#pragma once

#include "IntervalTree.h"
#include "IntervalTreeAction.h"
#include <unordered_set>
#include <map>

namespace IntervalTree
{
   struct Statistics
   {
      int Size = -1;
      std::vector<int> NumberOfLeavesPerLevel;
      std::string toString() const;
      auto operator<=>(const Statistics&) const = default;
   };

   template<int N>
   Statistics GetStatistics(const IndexTree<N>& tree)
   {
      Statistics result;

      ActionCountPerLevel<N> countPerLevel;
      tree. foreachLeaf(countPerLevel);
      result.Size = static_cast<int>(tree.size());

      result.NumberOfLeavesPerLevel = std::vector<int>(countPerLevel.Count.rbegin()->first + 1, 0);
      for (const auto& itr : countPerLevel.Count)
      {
         result.NumberOfLeavesPerLevel.at(itr.first) = itr.second;
      }

      return result;
   }

}
