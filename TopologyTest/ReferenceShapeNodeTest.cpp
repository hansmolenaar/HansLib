#include <gtest/gtest.h>
#include "Topology/ReferenceShapeNode.h"
#include  "Utilities/MyException.h"
#include "TestIReferenceShape.h"

TEST(ReferenceShapeNodeTest, ReferenceShapeNode)
{
   const ReferenceShapeNode rs;
   const auto& adjacencies = rs.getAdjacencies();
   ASSERT_EQ(adjacencies.getMaxTopologyDimension(), TopologyDimensionDef::Corner);
   ASSERT_EQ(adjacencies.getCount(TopologyDimensionDef::Corner), 1);
}


TEST(ReferenceShapeNodeTest, TestInterface)
{
   TopologyTest::TestIReferenceShape(ReferenceShapeNode());
}
