#include <gtest/gtest.h>

#include "Tree/KdTree.h"
#include "Point/Point.h"
#include "BoundingBox/BoundingBox.h"
#include "Tree/KdTreeTraversorPointsInRange.h"
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
   auto tree = KdTree<int, 1>::Create(values);
   ASSERT_EQ(tree->GetAllLeavesInOrder().size(), 0);

   const auto searchRange = BoundingBox<int, 1>::Create(std::array<int, 1>{ 0 });
   KdTreeTraversorPointsInRange<int, 1> traversor(searchRange);
   tree->Traverse(traversor);
   ASSERT_TRUE(traversor.GetFound().empty());
}


TEST(KdTreeTest, SingleElement)
{
   auto tree = KdTree<int, 1>::Create(std::array<IntPoint1, 1>{ IntPoint1{ 10 }});
   ASSERT_EQ(1, tree->GetAllLeavesInOrder().size());
   ASSERT_EQ(0, tree->GetAllLeavesInOrder().front());

   //var searchRange = BoundingBox<int>.CreateFromSinglePoint(new int[] { 0 });
   auto searchRange = BoundingBox<int, 1>::Create(std::vector<int>{0});
   ASSERT_TRUE(tree->FindInRange(searchRange).empty());
   searchRange = BoundingBox<int, 1>::CreateFromList(std::vector<IntPoint1>{ {0}, { 20 }});
   auto found = tree->FindInRange(searchRange);
   ASSERT_EQ(found.size(), 1);
   ASSERT_EQ(found.front(), 0);
}