#include <gtest/gtest.h>
#include "Topology/ReferenceShapeNode.h"
#include  "Utilities/MyException.h"

TEST(ReferenceShapeNodeTest, ReferenceShapeNode)
{
   const ReferenceShapeNode rs;
   const auto& adjacencies = rs.getAdjacencies();
   ASSERT_EQ(adjacencies.getMaxTopologyDimension(), TopologyDimension::Corner);
   ASSERT_EQ(adjacencies.getCount(TopologyDimension::Corner), 1);
   ASSERT_EQ(adjacencies.getCount(TopologyDimension::Edge), 0);
   ASSERT_THROW(adjacencies.getAdjacency(TopologyDimension::Edge, TopologyDimension::Corner), MyException);
}