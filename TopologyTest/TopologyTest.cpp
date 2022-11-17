#include <gtest/gtest.h>
#include "Topology/ReferenceShapeNode.h"
#include  "Utilities/MyException.h"

TEST(TopologyTest, ReferenceShapeNode)
{
   const ReferenceShapeNode rs;
   ASSERT_EQ(rs.getMaxTopologyDimension(), TopologyDimension::Corner);
   ASSERT_EQ(rs.getCount(TopologyDimension::Corner), 1);
   ASSERT_EQ(rs.getCount(TopologyDimension::Edge), 0);
   ASSERT_THROW(rs.getAdjacency(TopologyDimension::Edge, TopologyDimension::Corner), MyException);
}