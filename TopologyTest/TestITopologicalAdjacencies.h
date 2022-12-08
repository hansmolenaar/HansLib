#pragma once

#include <gtest/gtest.h>
#include "Topology/ITopologicalAdjacencies.h"
#include "Topology/ITopologicalAdjacency.h"
#include "TestITopologicalAdjacency.h"

namespace TopologyTest
{
   inline  void TestITopologicalAdjacencies(const ITopologicalAdjacencies& adjacencies)
   {
      const auto maxdim = adjacencies.getMaxTopologyDimension();
      for (auto dimH = TopologyDimension::Corner; dimH <= maxdim; ++dimH)
      {
         for (auto dimL = TopologyDimension::Corner; dimL < dimH; ++dimL)
         {
            const auto found = adjacencies.getAdjacency(dimH, dimL);
            if (found.first)
            {
               TestITopologicalAdjacency(*found.second, adjacencies.getCount(dimL), adjacencies.getCount(dimH));
            }
            const auto reverse = adjacencies.getAdjacency(dimL, dimH);
            ASSERT_EQ(found, reverse);
         }
         ASSERT_ANY_THROW(adjacencies.getAdjacency(dimH, dimH));
      }

      if (maxdim < TopologyDimension::Volume)
      {
         const auto nxtDim = static_cast<TopologyDimension>(static_cast<int>(maxdim) + 1);
         ASSERT_ANY_THROW(adjacencies.getCount(nxtDim));
      }
   }
}