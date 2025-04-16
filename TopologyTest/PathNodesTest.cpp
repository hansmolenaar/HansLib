#include "PathNodes.h"
#include <gtest/gtest.h>

using namespace Topology;

TEST(PathNodesTest, Constructor)
{
   ASSERT_MYEXCEPTION_MESSAGE(PathNodes(std::vector<NodeIndex>{}), "PathNodes::PathNodes too small: size=0");
   ASSERT_MYEXCEPTION_MESSAGE(PathNodes(std::vector<NodeIndex>{1, 2, 1}), "PathNodes::PathNodes has duplicates: 1");
   const PathNodes path(std::vector<NodeIndex>{ 7, 1, 5 });
   ASSERT_TRUE(str::equal(path, std::vector<NodeIndex>{5, 1, 7}));
   ASSERT_EQ(path.size(), 3);
}


TEST(PathNodesTest, StreamInsertion)
{
   std::ostringstream os;
   const PathNodes path(std::vector<NodeIndex>{ 2, 1, 5 });
   os << path;
   ASSERT_EQ(os.str(), "(2, 1, 5)");
}

TEST(PathNodesTest, GetEdges)
{
   const PathNodes path(std::vector<NodeIndex>{ 2, 1, 5 });
   const auto edges = path.getEdges();
   ASSERT_EQ(edges.size(), 2);
   ASSERT_EQ(edges[0], EdgeNodesDirected(2, 1));
}