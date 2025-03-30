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

TEST(TriangleNodesTest, ContainsEdge)
{
   const TriangleNodes triangle{ 1, 2, 3 };
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

TEST(TriangleNodesTest, OppositeNode)
{
   const TriangleNodes triangle{ 1, 2, 3 };
   ASSERT_EQ(triangle.oppositeNode({ 1,2 }), 3);
   ASSERT_EQ(triangle.oppositeNode({ 2,1 }), 3);
   ASSERT_EQ(triangle.oppositeNode({ 1,3 }), 2);
   ASSERT_EQ(triangle.oppositeNode({ 3,1 }), 2);
   ASSERT_EQ(triangle.oppositeNode({ 2,3 }), 1);
   ASSERT_EQ(triangle.oppositeNode({ 3,2 }), 1);

   ASSERT_THROW(triangle.oppositeNode({ 1,4 }), MyException);
   ASSERT_THROW(triangle.oppositeNode({ 4,1 }), MyException);
   ASSERT_THROW(triangle.oppositeNode({ 2,4 }), MyException);
   ASSERT_THROW(triangle.oppositeNode({ 4,2 }), MyException);
   ASSERT_THROW(triangle.oppositeNode({ 3,4 }), MyException);
   ASSERT_THROW(triangle.oppositeNode({ 4,3 }), MyException);
   ASSERT_THROW(triangle.oppositeNode({ 4,5 }), MyException);
}

TEST(TriangleNodesTest, StreamInsertion)
{
   std::ostringstream os;
   const TriangleNodes triangle{ 2,1,5 };
   os << triangle;
   ASSERT_EQ(os.str(), "(2, 1, 5)");
}
