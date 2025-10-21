#include "EdgeNodesSorted.h"
#include "Defines.h"
#include <algorithm>
#include <gtest/gtest.h>

using namespace Topology;

TEST(EdgeNodesSortedTest, Constructor)
{
    const std::array<NodeIndex, 2> expect{1, 2};
    EdgeNodesSorted edge{1, 2};
    ASSERT_TRUE(str::equal(edge, expect));
    edge = EdgeNodesSorted{2, 1};
    ASSERT_TRUE(str::equal(edge, expect));
}

TEST(EdgeNodesSortedTest, Equals)
{
    const EdgeNodesSorted edge{2, 1};
    ASSERT_TRUE(edge == edge);
    ASSERT_TRUE(edge == EdgeNodesSorted(2, 1));
    ASSERT_TRUE(edge == EdgeNodesSorted(1, 2));
    ASSERT_FALSE(edge == EdgeNodesSorted(1, 3));
}

TEST(EdgeNodesSortedTest, SpaceShip)
{
    const EdgeNodesSorted edge{1, 2};
    ASSERT_TRUE(edge <= EdgeNodesSorted(2, 1));
    ASSERT_TRUE(edge >= EdgeNodesSorted(2, 1));
    ASSERT_FALSE(edge < EdgeNodesSorted(2, 1));
    ASSERT_FALSE(edge < EdgeNodesSorted(1, 2));
    ASSERT_TRUE(edge < EdgeNodesSorted(3, 1));
}

TEST(EdgeNodesSortedTest, StreamInsertion)
{
    std::ostringstream os;
    const EdgeNodesSorted edge{2, 1};
    os << edge;
    ASSERT_EQ(os.str(), "(1, 2)");
}

TEST(EdgeNodesSortedTest, contains)
{
    EdgeNodesSorted edge{2, 1};
    ASSERT_TRUE(edge.contains(1));
    ASSERT_TRUE(edge.contains(2));
    ASSERT_FALSE(edge.contains(3));
}