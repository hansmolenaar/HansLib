#include "TriangleNodes.h"
#include <gtest/gtest.h>

using namespace Topology;

TEST(TriangleNodesTest, ContainsNode)
{
   const TriangleNodes triangle{ 1, 2, 3 };
   ASSERT_TRUE(TriangleNodesNodesContainsNode(triangle, 1));
   ASSERT_TRUE(TriangleNodesNodesContainsNode(triangle, 2));
   ASSERT_TRUE(TriangleNodesNodesContainsNode(triangle, 3));
   ASSERT_FALSE(TriangleNodesNodesContainsNode(triangle, 4));
   ASSERT_FALSE(TriangleNodesNodesContainsNode(triangle, NodeIndexInvalid));
}

TEST(TriangleNodesTest, ContainsEdge)
{
   const TriangleNodes triangle{ 1, 2, 3 };
   ASSERT_TRUE(TriangleNodesNodesContainsEdge(triangle, { 1,2 }));
   ASSERT_TRUE(TriangleNodesNodesContainsEdge(triangle, { 2,1 }));
   ASSERT_TRUE(TriangleNodesNodesContainsEdge(triangle, { 1,3 }));
   ASSERT_TRUE(TriangleNodesNodesContainsEdge(triangle, { 3,1 }));
   ASSERT_TRUE(TriangleNodesNodesContainsEdge(triangle, { 2,3 }));
   ASSERT_TRUE(TriangleNodesNodesContainsEdge(triangle, { 3,2 }));

   ASSERT_FALSE(TriangleNodesNodesContainsEdge(triangle, { 1,4 }));
   ASSERT_FALSE(TriangleNodesNodesContainsEdge(triangle, { 2,4 }));
   ASSERT_FALSE(TriangleNodesNodesContainsEdge(triangle, { 3,4 }));
}

TEST(TriangleNodesTest, OppositeNode)
{
   const TriangleNodes triangle{ 1, 2, 3 };
   ASSERT_EQ(TriangleNodesNodesOppositeNode(triangle, { 1,2 }), 3);
   ASSERT_EQ(TriangleNodesNodesOppositeNode(triangle, { 2,1 }), 3);
   ASSERT_EQ(TriangleNodesNodesOppositeNode(triangle, { 1,3 }), 2);
   ASSERT_EQ(TriangleNodesNodesOppositeNode(triangle, { 3,1 }), 2);
   ASSERT_EQ(TriangleNodesNodesOppositeNode(triangle, { 2,3 }), 1);
   ASSERT_EQ(TriangleNodesNodesOppositeNode(triangle, { 3,2 }), 1);

   ASSERT_THROW(TriangleNodesNodesOppositeNode(triangle, { 1,4 }), MyException);
   ASSERT_THROW(TriangleNodesNodesOppositeNode(triangle, { 4,1 }), MyException);
   ASSERT_THROW(TriangleNodesNodesOppositeNode(triangle, { 2,4 }), MyException);
   ASSERT_THROW(TriangleNodesNodesOppositeNode(triangle, { 4,2 }), MyException);
   ASSERT_THROW(TriangleNodesNodesOppositeNode(triangle, { 3,4 }), MyException);
   ASSERT_THROW(TriangleNodesNodesOppositeNode(triangle, { 4,3 }), MyException);
   ASSERT_THROW(TriangleNodesNodesOppositeNode(triangle, { 4,5 }), MyException);
}