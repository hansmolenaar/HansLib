#include "DirectedEdgeNodes.h"
#include <gtest/gtest.h>
#include <sstream>

using namespace Topology;

TEST(DirectedEdgeNodesTest, Contains)
{
   const DirectedEdgeNodes edge{ 2,1 };
   ASSERT_TRUE(edge.contains(1));
   ASSERT_TRUE(edge.contains(2));
   ASSERT_FALSE(edge.contains(3));
   ASSERT_FALSE(edge.contains(NodeIndexInvalid));
}

TEST(DirectedEdgeNodesTest, Equals)
{
   const DirectedEdgeNodes edge{ 2,1 };
   ASSERT_TRUE(edge == edge);
   ASSERT_TRUE(edge == DirectedEdgeNodes(2, 1));
   ASSERT_FALSE(edge == DirectedEdgeNodes(1, 2));
}

TEST(DirectedEdgeNodesTest, SpaceShip)
{
   const DirectedEdgeNodes edge{ 2,1 };
   ASSERT_TRUE(edge <= DirectedEdgeNodes(2, 1));
   ASSERT_TRUE(edge >= DirectedEdgeNodes(2, 1));
   ASSERT_FALSE(edge < DirectedEdgeNodes(2, 1));
   ASSERT_FALSE(edge < DirectedEdgeNodes(1, 2));
}

TEST(DirectedEdgeNodesTest, StreamInsertion)
{
   std::ostringstream os;
   const DirectedEdgeNodes edge{ 2,1 };
   os << edge;
   ASSERT_EQ(os.str(), "(2 -> 1)");
}
