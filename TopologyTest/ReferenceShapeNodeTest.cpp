#include <gtest/gtest.h>
#include "Topology/ReferenceShapeNode.h"
#include  "Utilities/MyException.h"
#include "TestITopologicalAdjacencies.h"

TEST(ReferenceShapeNodeTest, ReferenceShapeNode)
{
   const ReferenceShapeNode rs;
   const auto& adjacencies = rs.getAdjacencies();
   ASSERT_EQ(adjacencies.getMaxTopologyDimension(), TopologyDimension::Corner);
   ASSERT_EQ(adjacencies.getCount(TopologyDimension::Corner), 1);
   ASSERT_FALSE(adjacencies.getAdjacency(TopologyDimension::Edge, TopologyDimension::Corner).first);
}


TEST(ReferenceShapeNodeTest, TestInterface)
{
   const ReferenceShapeNode rs;
   TopologyTest::TestITopologicalAdjacencies(rs.getAdjacencies());
}
