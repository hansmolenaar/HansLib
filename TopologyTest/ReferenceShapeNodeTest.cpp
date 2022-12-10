#include <gtest/gtest.h>
#include "Topology/ReferenceShapeNode.h"
#include  "Utilities/MyException.h"
#include "TestITopologicalAdjacencies.h"

TEST(ReferenceShapeNodeTest, ReferenceShapeNode)
{
   const ReferenceShapeNode rs;
   const auto& adjacencies = rs.getAdjacencies();
   ASSERT_EQ(adjacencies.getMaxTopologyDimension(), TopologyDimensionDef::Corner);
   ASSERT_EQ(adjacencies.getCount(TopologyDimensionDef::Corner), 1);
   ASSERT_FALSE(adjacencies.getAdjacency(TopologyDimensionDef::Edge, TopologyDimensionDef::Corner).first);
}


TEST(ReferenceShapeNodeTest, TestInterface)
{
   const ReferenceShapeNode rs;
   TopologyTest::TestITopologicalAdjacencies(rs.getAdjacencies());
}
