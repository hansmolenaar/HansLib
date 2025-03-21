#include "DirectedEdgeNodes.h"
#include <gtest/gtest.h>

using namespace Topology;

TEST(DirectedEdgeNodesTest, Sort)
{
   DirectedEdgeNodes edge{ 2,1 };

   DirectedEdgeNodesSort(edge);
   ASSERT_EQ(edge[0], 1);
   ASSERT_EQ(edge[1], 2);

   DirectedEdgeNodesSort(edge);
   ASSERT_EQ(edge[0], 1);
   ASSERT_EQ(edge[1], 2);
}

TEST(DirectedEdgeNodesTest, Contains)
{
   const DirectedEdgeNodes edge{ 2,1 };
   ASSERT_TRUE(DirectedEdgeNodesContains(edge, 1));
   ASSERT_TRUE(DirectedEdgeNodesContains(edge, 2));
   ASSERT_FALSE(DirectedEdgeNodesContains(edge, 3));
   ASSERT_FALSE(DirectedEdgeNodesContains(edge, NodeIndexInvalid));
}
