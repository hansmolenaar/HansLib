#pragma once

#include <gtest/gtest.h>
#include "Topology/IReferenceShape.h"
#include "TopologyTest/TestITopologicalAdjacencies.h"
#include <set>

namespace TopologyTest
{
   inline  void TestIReferenceShape(const IReferenceShape& referenceShape)
   {
      const auto& adjacencies = referenceShape.getAdjacencies();
      const TopologyDimension maxdim = adjacencies.getMaxTopologyDimension();
      ASSERT_EQ(adjacencies.getCount(maxdim), 1);
      if (maxdim > TopologyDimensionDef::Corner)
      {
         ASSERT_TRUE(adjacencies.getAdjacency(maxdim, TopologyDimensionDef::Corner).first);
      }
      TopologyTest::TestITopologicalAdjacencies(adjacencies);
   }
}
