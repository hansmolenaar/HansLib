#include "DirectedEdgeNodes.h"
#include <gtest/gtest.h>

using namespace Topology;

TEST(DirectedEdgeNodesTest, Contains)
{
   const DirectedEdgeNodes edge{ 2,1 };
   ASSERT_TRUE(edge.contains(1));
   ASSERT_TRUE(edge.contains(2));
   ASSERT_FALSE(edge.contains(3));
   ASSERT_FALSE(edge.contains(NodeIndexInvalid));
}
