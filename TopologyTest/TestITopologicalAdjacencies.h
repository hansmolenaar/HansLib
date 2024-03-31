#pragma once

#include <gtest/gtest.h>
#include "ITopologicalAdjacencies.h"
#include "ITopologicalAdjacency.h"
#include "TestITopologicalAdjacency.h"

using namespace Topology;

namespace TopologyTest
{
   inline  void TestITopologicalAdjacencies(const ITopologicalAdjacencies& adjacencies)
   {
      const auto maxdim = adjacencies.getMaxTopologyDimension();
      for (auto dimH = Topology::Corner; dimH <= maxdim; ++dimH)
      {
         for (auto dimL = Topology::Corner; dimL < dimH; ++dimL)
         {
            const auto found = adjacencies.getAdjacency(dimH, dimL);
            if (found)
            {
               ASSERT_EQ(found.value()->getDimensionLow(), dimL);
               ASSERT_EQ(found.value()->getDimensionHigh(), dimH);
               TestITopologicalAdjacency(**found, adjacencies.getCountSafe(dimL), adjacencies.getCountSafe(dimH));
            }
            const auto reverse = adjacencies.getAdjacency(dimL, dimH);
            ASSERT_EQ(found, reverse);
         }
         ASSERT_ANY_THROW(adjacencies.getAdjacency(dimH, dimH));
      }

      const auto nxtDim = static_cast<TopologyDimension>(static_cast<int>(maxdim) + 1);
      ASSERT_ANY_THROW(adjacencies.getCountSafe(nxtDim));

      ASSERT_ANY_THROW(adjacencies.getAdjacency(Topology::Corner, nxtDim));
      ASSERT_ANY_THROW(adjacencies.getAdjacency(nxtDim, Topology::Corner));
   }
}