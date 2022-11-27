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
            TestITopologicalAdjacency(adjacencies.getAdjacency(dimH, dimL), adjacencies.getCount(dimL), adjacencies.getCount(dimH));
         }
         ASSERT_ANY_THROW(adjacencies.getAdjacency(dimH, dimH));
      }
   }
}