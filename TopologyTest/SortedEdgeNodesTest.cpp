#include "Defines.h"
#include "SortedEdgeNodes.h"
#include <algorithm>
#include <gtest/gtest.h>

using namespace Topology;

TEST(SortedEdgeNodestest, Constructor)
{
   const std::array<NodeIndex, 2> expect{ 1,2 };
   SortedEdgeNodes edge{ 1,2 };
   ASSERT_TRUE(str::equal(edge, expect));
   edge = SortedEdgeNodes{ 2,1 };
   ASSERT_TRUE(str::equal(edge, expect));
}

TEST(SortedEdgeNodestest, Equals)
{
   const SortedEdgeNodes edge{ 2,1 };
   ASSERT_TRUE(edge == edge);
   ASSERT_TRUE(edge == SortedEdgeNodes(2, 1));
   ASSERT_TRUE(edge == SortedEdgeNodes(1, 2));
   ASSERT_FALSE(edge == SortedEdgeNodes(1, 3));
}

TEST(SortedEdgeNodestest, SpaceShip)
{
   const SortedEdgeNodes edge{ 1, 2 };
   ASSERT_TRUE(edge <= SortedEdgeNodes(2, 1));
   ASSERT_TRUE(edge >= SortedEdgeNodes(2, 1));
   ASSERT_FALSE(edge < SortedEdgeNodes(2, 1));
   ASSERT_FALSE(edge < SortedEdgeNodes(1, 2));
   ASSERT_TRUE(edge < SortedEdgeNodes(3, 1));
}
