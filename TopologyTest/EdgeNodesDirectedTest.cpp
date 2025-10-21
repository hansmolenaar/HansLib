#include "EdgeNodesDirected.h"
#include <gtest/gtest.h>
#include <sstream>

using namespace Topology;

TEST(EdgeNodesDirectedTest, Contains)
{
    const EdgeNodesDirected edge{2, 1};
    ASSERT_TRUE(edge.contains(1));
    ASSERT_TRUE(edge.contains(2));
    ASSERT_FALSE(edge.contains(3));
    ASSERT_FALSE(edge.contains(NodeIndexInvalid));
}

TEST(EdgeNodesDirectedTest, Equals)
{
    const EdgeNodesDirected edge{2, 1};
    ASSERT_TRUE(edge == edge);
    ASSERT_TRUE(edge == EdgeNodesDirected(2, 1));
    ASSERT_FALSE(edge == EdgeNodesDirected(1, 2));
}

TEST(EdgeNodesDirectedTest, SpaceShip)
{
    const EdgeNodesDirected edge{2, 1};
    ASSERT_TRUE(edge <= EdgeNodesDirected(2, 1));
    ASSERT_TRUE(edge >= EdgeNodesDirected(2, 1));
    ASSERT_FALSE(edge < EdgeNodesDirected(2, 1));
    ASSERT_FALSE(edge < EdgeNodesDirected(1, 2));
}

TEST(EdgeNodesDirectedTest, StreamInsertion)
{
    std::ostringstream os;
    const EdgeNodesDirected edge{2, 1};
    os << edge;
    ASSERT_EQ(os.str(), "(2 -> 1)");
}
