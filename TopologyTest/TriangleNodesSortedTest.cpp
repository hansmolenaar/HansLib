#include "TriangleNodesSorted.h"
#include <gtest/gtest.h>

using namespace Topology;

TEST(TriangleNodesSortedTest, ContainsNode)
{
   const TriangleNodesSorted triangle{ 3, 2, 1 };
   ASSERT_TRUE(triangle.contains(1));
   ASSERT_TRUE(triangle.contains(2));
   ASSERT_TRUE(triangle.contains(3));
   ASSERT_FALSE(triangle.contains(4));
   ASSERT_FALSE(triangle.contains(NodeIndexInvalid));
}

TEST(TriangleNodesSortedTest, ContainsEdge)
{
   const TriangleNodesSorted triangle{ 2, 1, 3 };
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

TEST(TriangleNodesSortedTest, OppositeNode)
{
   const TriangleNodesSorted triangle{ 3, 1, 2 };
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

TEST(TriangleNodesSortedTest, CreateSorted)
{
   const std::array<NodeIndex, 3> expect{ 1,2,3 };
   TriangleNodesSorted triangle(3, 2, 1);
   ASSERT_TRUE(str::equal(triangle, expect));
   triangle = TriangleNodesSorted(2, 1, 3);
   ASSERT_TRUE(str::equal(triangle, expect));
   triangle = TriangleNodesSorted(3, 1, 2);
   ASSERT_TRUE(str::equal(triangle, expect));
   triangle = TriangleNodesSorted(2, 3, 1);
   ASSERT_TRUE(str::equal(triangle, expect));
}


TEST(TriangleNodesSortedTest, StreamInsertion)
{
   std::ostringstream os;
   const TriangleNodesSorted edge{ 2,1,5 };
   os << edge;
   ASSERT_EQ(os.str(), "(1, 2, 5)");
}
