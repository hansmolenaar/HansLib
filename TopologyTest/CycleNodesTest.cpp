#include "CycleNodes.h"
#include <gtest/gtest.h>

using namespace Topology;

TEST(CycleNodesTest, Constructor)
{
   ASSERT_MYEXCEPTION_MESSAGE(CycleNodes(std::vector<NodeIndex>{1, 2}), "CycleNodes::CycleNodes too small: size=2");
   ASSERT_MYEXCEPTION_MESSAGE(CycleNodes(std::vector<NodeIndex>{1, 2, 3, 2}), "CycleNodes::CycleNodes has duplicates: 2");
   const CycleNodes cycle(std::vector<NodeIndex>{ 7, 1, 5 });
   ASSERT_TRUE(str::equal(cycle, std::vector<NodeIndex>{1, 5, 7}));
   ASSERT_EQ(cycle.size(), 3);
}


TEST(CycleNodesTest, StreamInsertion)
{
   std::ostringstream os;
   const CycleNodes cycle(std::vector<NodeIndex>{ 2, 3, 5 });
   os << cycle;
   ASSERT_EQ(os.str(), "(2, 3, 5)");
}

TEST(CycleNodesTest, GetEdges)
{
   const CycleNodes cycle(std::vector<NodeIndex>{ 2, 1, 5 });
   const auto edges = cycle.getEdges();
   ASSERT_EQ(edges.size(), 3);
   ASSERT_EQ(edges[0], EdgeNodesDirected(1, 5));
   ASSERT_EQ(edges[1], EdgeNodesDirected(5, 2));
   ASSERT_EQ(edges[2], EdgeNodesDirected(2, 1));
}