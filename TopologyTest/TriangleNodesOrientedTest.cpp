#include "TriangleNodesOriented.h"
#include <gtest/gtest.h>

using namespace Topology;

TEST(TriangleNodesOrientedTest, ContainsNode)
{
   const TriangleNodesOriented triangle{ 3, 2, 1 };
   ASSERT_TRUE(triangle.contains(1));
   ASSERT_TRUE(triangle.contains(2));
   ASSERT_TRUE(triangle.contains(3));
   ASSERT_FALSE(triangle.contains(4));
   ASSERT_FALSE(triangle.contains(NodeIndexInvalid));
}

TEST(TriangleNodesOrientedTest, ContainsEdge)
{
   const TriangleNodesOriented triangle{ 2, 1, 3 };
   ASSERT_TRUE(triangle.contains({ 1,2 }));
   ASSERT_TRUE(triangle.contains({ 2,1 }));
   ASSERT_TRUE(triangle.contains({ 1,3 }));
   ASSERT_TRUE(triangle.contains({ 3,1 }));
   ASSERT_TRUE(triangle.contains({ 2,3 }));
   ASSERT_TRUE(triangle.contains({ 3,2 }));

   ASSERT_FALSE(triangle.contains({ 1,4 }));
   ASSERT_FALSE(triangle.contains({ 2,4 }));
   ASSERT_FALSE(triangle.contains({ 3,4 }));
}

TEST(TriangleNodesOrientedTest, OppositeNode)
{
   const TriangleNodesOriented triangle{ 3, 1, 2 };
   ASSERT_EQ(triangle.oppositeNode({ 1,2 }), 3);
   ASSERT_EQ(triangle.oppositeNode({ 2,1 }), 3);
   ASSERT_EQ(triangle.oppositeNode({ 1,3 }), 2);
   ASSERT_EQ(triangle.oppositeNode({ 3,1 }), 2);
   ASSERT_EQ(triangle.oppositeNode({ 2,3 }), 1);
   ASSERT_EQ(triangle.oppositeNode({ 3,2 }), 1);

   ASSERT_MYEXCEPTION_MESSAGE(triangle.oppositeNode({ 1,4 }), "Triangle does not contain edge");
   ASSERT_MYEXCEPTION_MESSAGE(triangle.oppositeNode({ 4,1 }), "Triangle does not contain edge");
   ASSERT_MYEXCEPTION_MESSAGE(triangle.oppositeNode({ 2,4 }), "Triangle does not contain edge");
   ASSERT_MYEXCEPTION_MESSAGE(triangle.oppositeNode({ 4,2 }), "Triangle does not contain edge");
   ASSERT_MYEXCEPTION_MESSAGE(triangle.oppositeNode({ 3,4 }), "Triangle does not contain edge");
   ASSERT_MYEXCEPTION_MESSAGE(triangle.oppositeNode({ 4,3 }), "Triangle does not contain edge");
   ASSERT_MYEXCEPTION_MESSAGE(triangle.oppositeNode({ 4,5 }), "Triangle does not contain edge");
}

TEST(TriangleNodesOrientedTest, Find)
{
   const TriangleNodesOriented triangle{ 3, 4, 5 };
   ASSERT_EQ(triangle.find(3), 0);
   ASSERT_EQ(triangle.find(4), 1);
   ASSERT_EQ(triangle.find(5), 2);
   ASSERT_EQ(triangle.find(6), std::numeric_limits<size_t>::max());
}

TEST(TriangleNodesOrientedTest, CreateOriented)
{
   std::array<NodeIndex, 3> nodes{ 1,2,3 };

   const TriangleNodesOriented expect(1, 2, 3);

   TriangleNodesOriented triangle(nodes[0], nodes[1], nodes[2]);
   ASSERT_TRUE(str::equal(triangle, expect));

   str::rotate(nodes, nodes.begin() + 1);
   triangle = TriangleNodesOriented(nodes[0], nodes[1], nodes[2]);
   ASSERT_TRUE(str::equal(triangle, expect));

   str::rotate(nodes, nodes.begin() + 1);
   triangle = TriangleNodesOriented(nodes[0], nodes[1], nodes[2]);
   ASSERT_TRUE(str::equal(triangle, expect));
}

TEST(TriangleNodesOrientedTest, StreamInsertion)
{
   std::ostringstream os;
   const TriangleNodesOriented triangle{ 2,1,5 };
   os << triangle;
   ASSERT_EQ(os.str(), "(1, 5, 2)");
}

TEST(TriangleNodesOrientedTest, GetEdges)
{
   const TriangleNodesOriented triangle{ 2,1,5 };
   const auto edges = triangle.getEdges();
   ASSERT_EQ(edges.size(), 3);
   for (auto edge : edges)
   {
      ASSERT_TRUE(triangle.contains(edge));
   }
}
