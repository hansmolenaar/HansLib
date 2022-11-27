#pragma once

#include <gtest/gtest.h>
#include "Topology/ITopologicalAdjacency.h"
#include <set>

namespace TopologyTest
{
   inline  void TestITopologicalAdjacency(const ITopologicalAdjacency& adjacency, int countLow, int countHig)
   {
      const auto dimLow = adjacency.getDimensionLow();
      const auto dimHig = adjacency.getDimensionHigh();

      ASSERT_TRUE(dimHig > dimLow);
      std::set<std::pair<int, int>> l2h;
      for (auto low = 0; low < countLow; ++low)
      {
         const auto& connectedHigh = adjacency.getConnectedHighers(low);
         for (auto hig : connectedHigh)
         {
            const std::pair<int, int> pair = std::make_pair(low, hig);
            ASSERT_FALSE(l2h.contains(pair));
            l2h.insert(pair);
         }
      }
      for (auto hig = 0; hig < countHig; ++hig)
      {
         const auto& connectedLow = adjacency.getConnectedHighers(hig);
         for (auto low : connectedLow)
         {
            const std::pair<int, int> pair = std::make_pair(low, hig);
            ASSERT_TRUE(l2h.contains(pair));
            l2h.erase(pair);
         }
      }
      ASSERT_TRUE(l2h.empty());

      ASSERT_ANY_THROW(adjacency.getConnectedLowers(countHig));
      ASSERT_ANY_THROW(adjacency.getConnectedHighers(countLow));
   }
}
