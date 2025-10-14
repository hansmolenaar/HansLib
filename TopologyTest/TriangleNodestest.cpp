#include "TriangleNodes.h"
#include <gtest/gtest.h>

using namespace Topology;

TEST(TriangleNodesTest, ContainsNode)
{
   const TriangleNodes triangle{ 1, 2, 3 };
   ASSERT_TRUE(triangle.contains(1));
   ASSERT_TRUE(triangle.contains(2));
   ASSERT_TRUE(triangle.contains(3));
   ASSERT_FALSE(triangle.contains(4));
   ASSERT_FALSE(triangle.contains(NodeIndexInvalid));
}

TEST(TriangleNodesTest, ContainsDirectedEdge)
{
   const TriangleNodes triangle{ 1, 2, 3 };
   ASSERT_TRUE(triangle.containsNodes(1, 2));
   ASSERT_TRUE(triangle.containsNodes(2, 1));
   ASSERT_TRUE(triangle.containsNodes(1, 3));
   ASSERT_TRUE(triangle.containsNodes(3, 1));
   ASSERT_TRUE(triangle.containsNodes(2, 3));
   ASSERT_TRUE(triangle.containsNodes(3, 2));

   ASSERT_FALSE(triangle.containsNodes(1, 4));
   ASSERT_FALSE(triangle.containsNodes(2, 4));
   ASSERT_FALSE(triangle.containsNodes(3, 4));
}

TEST(TriangleNodesTest, OppositeNode)
{
   const TriangleNodes triangle{ 1, 2, 3 };
   ASSERT_EQ(triangle.oppositeNode(EdgeNodesSorted{ 1,2 }), 3);
   ASSERT_EQ(triangle.oppositeNode(EdgeNodesDirected{ 2,1 }), 3);
   ASSERT_EQ(triangle.oppositeNode(EdgeNodesSorted{ 1,3 }), 2);
   ASSERT_EQ(triangle.oppositeNode(EdgeNodesDirected{ 3,1 }), 2);
   ASSERT_EQ(triangle.oppositeNode(EdgeNodesSorted{ 2,3 }), 1);
   ASSERT_EQ(triangle.oppositeNode(EdgeNodesDirected{ 3,2 }), 1);

   ASSERT_MYEXCEPTION_MESSAGE(triangle.oppositeNode(EdgeNodesSorted{ 1,4 }), "Triangle does not contain edge");
   ASSERT_MYEXCEPTION_MESSAGE(triangle.oppositeNode(EdgeNodesDirected{ 4,1 }), "Triangle does not contain edge");
   ASSERT_MYEXCEPTION_MESSAGE(triangle.oppositeNode(EdgeNodesSorted{ 2,4 }), "Triangle does not contain edge");
   ASSERT_MYEXCEPTION_MESSAGE(triangle.oppositeNode(EdgeNodesDirected{ 4,2 }), "Triangle does not contain edge");
   ASSERT_MYEXCEPTION_MESSAGE(triangle.oppositeNode(EdgeNodesSorted{ 3,4 }), "Triangle does not contain edge");
   ASSERT_MYEXCEPTION_MESSAGE(triangle.oppositeNode(EdgeNodesDirected{ 4,3 }), "Triangle does not contain edge");
   ASSERT_MYEXCEPTION_MESSAGE(triangle.oppositeNode(EdgeNodesSorted{ 4,5 }), "Triangle does not contain edge");
}

TEST(TriangleNodesTest, StreamInsertion)
{
   std::ostringstream os;
   const TriangleNodes triangle{ 2,1,5 };
   os << triangle;
   ASSERT_EQ(os.str(), "(2, 1, 5)");
}

TEST(TriangleNodesTest, GetEdges)
{
   const TriangleNodes triangle{ 2,1,5 };
   const auto edges = triangle.getEdges();
   ASSERT_EQ(edges.size(), 3);
   for (auto edge : edges)
   {
      ASSERT_TRUE(triangle.containsNodes(edge[0], edge[1]));
   }
}