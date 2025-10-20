#include "ReferenceShapeNode.h"
#include "MyException.h"
#include "TestIReferenceShape.h"
#include <gtest/gtest.h>

TEST(ReferenceShapeNodeTest, ReferenceShapeNode)
{
    const ReferenceShapeNode rs;
    const auto &adjacencies = rs.getAdjacencies();
    ASSERT_EQ(adjacencies.getMaxTopologyDimension(), Topology::Corner);
    ASSERT_EQ(adjacencies.getCountSafe(Topology::Corner), 1);
}

TEST(ReferenceShapeNodeTest, TestInterface)
{
    TopologyTest::TestIReferenceShape(ReferenceShapeNode());
}
