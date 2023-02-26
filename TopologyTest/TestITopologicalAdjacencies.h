#pragma once

#include <gtest/gtest.h>
#include "ITopologicalAdjacencies.h"
#include "ITopologicalAdjacency.h"
#include "TestITopologicalAdjacency.h"

namespace TopologyTest
{
   inline  void TestITopologicalAdjacencies(const ITopologicalAdjacencies& adjacencies)
   {
      const auto maxdim = adjacencies.getMaxTopologyDimension();
      for (auto dimH = TopologyDimensionDef::Corner; dimH <= maxdim; ++dimH)
      {
         for (auto dimL = TopologyDimensionDef::Corner; dimL < dimH; ++dimL)
         {
            const auto found = adjacencies.getAdjacency(dimH, dimL);
            if (found.first)
            {
               ASSERT_EQ(found.second->getDimensionLow(), dimL);
               ASSERT_EQ(found.second->getDimensionHigh(), dimH);
               TestITopologicalAdjacency(*found.second, adjacencies.getCountSafe(dimL), adjacencies.getCountSafe(dimH));
            }
            const auto reverse = adjacencies.getAdjacency(dimL, dimH);
            ASSERT_EQ(found, reverse);
         }
         ASSERT_ANY_THROW(adjacencies.getAdjacency(dimH, dimH));
      }

      const auto nxtDim = static_cast<TopologyDimension>(static_cast<int>(maxdim) + 1);
      ASSERT_ANY_THROW(adjacencies.getCountSafe(nxtDim));

      ASSERT_ANY_THROW(adjacencies.getAdjacency(TopologyDimensionDef::Corner, nxtDim));
      ASSERT_ANY_THROW(adjacencies.getAdjacency(nxtDim, TopologyDimensionDef::Corner));
   }
}