#include <gtest/gtest.h>

#include "Tree/KdTree.h"
#include "Point/Point.h"
TEST(KdTreeTest, Vertex)
{
   KdTreeVertex<int, 1> vertex(nullptr, nullptr, 1, 2);
}

TEST(KdTreeTest, TestPreSorting)
{
   const std::vector<IntPoint2> points{ { 0, 1},{ 2, 0} };
   const PreSorting<int, 2> preSorting0(0, points);
   std::vector<KdTreePosition> order{ 0,1 };
   std::sort(order.begin(), order.end(), [&preSorting0](KdTreePosition p0, KdTreePosition p1) {return preSorting0.less(p0, p1); });
   std::vector<KdTreePosition> expect{ 1,0 };
   ASSERT_TRUE(std::equal(order.begin(), order.end(), expect.begin(), expect.end()));

   const PreSorting<int, 2> preSorting1(1, points);
   std::sort(order.begin(), order.end(), [&preSorting1](KdTreePosition p0, KdTreePosition p1) {return preSorting1.less(p0, p1); });
   expect = std::vector<KdTreePosition>{ 0,1 };
   ASSERT_TRUE(std::equal(order.begin(), order.end(), expect.begin(), expect.end()));
}

TEST(KdTreeTest, EmptyTree)
{
   std::vector<IntPoint1> values;
   auto tree = KdTree<int,1>::Create(values);
   ASSERT_EQ(tree->GetAllLeavesInOrder().size(), 0);

  // var searchRange = BoundingBox<int>.CreateFromSinglePoint(new int[] { 0 });
  // Assert.IsFalse(tree.FindInRange(searchRange).Any());
}