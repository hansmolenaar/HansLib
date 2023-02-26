#pragma once

#include <gtest/gtest.h>
#include "IReferenceShape.h"
#include "TestITopologicalAdjacencies.h"
#include <set>

namespace TopologyTest
{
   inline  void TestIReferenceShape(const IReferenceShape& referenceShape)
   {
      const auto& adjacencies = referenceShape.getAdjacencies();
      const TopologyDimension maxdim = adjacencies.getMaxTopologyDimension();
      ASSERT_EQ(adjacencies.getCountSafe(maxdim), 1);
      if (maxdim > TopologyDimensionDef::Corner)
      {
         ASSERT_TRUE(adjacencies.getAdjacency(maxdim, TopologyDimensionDef::Corner).first);
      }
      TopologyTest::TestITopologicalAdjacencies(adjacencies);
   }
}
