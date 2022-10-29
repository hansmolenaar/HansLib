#include <gtest/gtest.h>

#include "Tree/KdTree.h"
#include "Point/Point.h"
#include "BoundingBox/BoundingBox.h"
#include "Tree/KdTreeTraversorPointsInRange.h"

#include <random>
#include <algorithm>

namespace str = std::ranges;

template<int N>
static void TestIntKdTree(std::span<const Point<int, N>> points)
{
   if (points.empty()) return;
   const size_t numPoints = points.size();
   constexpr int maxShuffles = 10;
   const auto searchRange = BoundingBox<int, N>::Create(points.front());
   const auto expectToFind = std::count_if(points.begin(), points.end(), [&points](const Point<int, N>& p) { return p == points.front(); });

   std::vector<int> position(numPoints);
   for (size_t n = 0; n < points.size(); ++n)
   {
      position.at(n) = n;
   }

   std::random_device rd;
   std::mt19937 g(rd());

   for (int shuffle = 0; shuffle < maxShuffles; ++shuffle)
   {
      std::shuffle(position.begin(), position.end(), g);
      std::vector<  Point<int, N>> shuffledPoints;
      for (size_t n = 0; n < numPoints; ++n)
      {
         shuffledPoints.push_back(points[position[n]]);
      }
      const auto tree = KdTree<int, N>::Create(shuffledPoints);
      const auto found = tree->FindInRange(searchRange);
      const std::set<KdTreePosition> foundSet(found.begin(), found.end());
      ASSERT_EQ(foundSet.size(), expectToFind);
      for (auto f : foundSet)
      {
         ASSERT_TRUE(shuffledPoints.at(f) == points.front());
      }
   }

}

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
   std::vector<KdTreePosition> expect{ 0,1 };
   ASSERT_TRUE(std::equal(order.begin(), order.end(), expect.begin(), expect.end()));

   const PreSorting<int, 2> preSorting1(1, points);
   std::sort(order.begin(), order.end(), [&preSorting1](KdTreePosition p0, KdTreePosition p1) {return preSorting1.less(p0, p1); });
   expect = std::vector<KdTreePosition>{ 1,0 };
   ASSERT_TRUE(std::equal(order.begin(), order.end(), expect.begin(), expect.end()));
}

TEST(KdTreeTest, TestPreSorting2)
{
   const std::vector<IntPoint2> points{ { 0, 2},{ 0, 1} };
   const PreSorting<int, 2> preSorting0(0, points);
   std::vector<KdTreePosition> order{ 0,1 };
   std::sort(order.begin(), order.end(), [&preSorting0](KdTreePosition p0, KdTreePosition p1) {return preSorting0.less(p0, p1); });
   std::vector<KdTreePosition> expect{ 1,0 };
   ASSERT_TRUE(std::equal(order.begin(), order.end(), expect.begin(), expect.end()));
}

TEST(KdTreeTest, TestPreSorting3)
{
   const std::vector<IntPoint2> points{ { 2, 0},{ 0, 0} };
   const PreSorting<int, 2> preSorting0(1, points);
   std::vector<KdTreePosition> order{ 0,1 };
   std::sort(order.begin(), order.end(), [&preSorting0](KdTreePosition p0, KdTreePosition p1) {return preSorting0.less(p0, p1); });
   std::vector<KdTreePosition> expect{ 1,0 };
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



TEST(KdTreeTest, TwoElements)
{
   auto tree = KdTree<int, 1>::Create(std::array<IntPoint1, 2>{ IntPoint1{ 10 }, IntPoint1{ 5 } });
   const auto ordered = tree->GetAllLeavesInOrder();
   const std::vector<KdTreePosition> expect{ 1,0 };
   ASSERT_TRUE(str::equal(ordered, expect));
}


TEST(KdTreeTest, ThreeElements1D)
{
   std::array<IntPoint1, 3> points{ IntPoint1{ 9 }, IntPoint1{ 5 }, IntPoint1{ 7 } };
   auto tree = KdTree<int, 1>::Create(points);
   const auto& orderedLeaves = tree->GetAllLeavesInOrder();
   ASSERT_TRUE(str::equal(orderedLeaves, std::vector<KdTreePosition>{1, 2, 0}));
   TestIntKdTree<1>(points);
}

TEST(KdTreeTest, TenPoints2D)
{
   constexpr int numPoints = 10;

   std::vector<IntPoint2> pairs(numPoints);
   for (int p = 0; p < numPoints; ++p)
   {
      pairs[p] = IntPoint2{ p, numPoints - p };
   }

   auto tree = KdTree<int, 2>::Create(pairs);
   const auto& orderedLeaves = tree->GetAllLeavesInOrder();
   ASSERT_TRUE(str::equal(orderedLeaves, std::vector<KdTreePosition>{3, 4, 0, 2, 1, 8, 9, 5, 7, 6}));

   TestIntKdTree<2>(pairs);
}


TEST(KdTreeTest, Duplicates)
{
   for (int numPoints = 11; numPoints < 20; ++numPoints)
   {
      std::vector<IntPoint2> points(numPoints);
      for (int p = 0; p < numPoints; ++p)
      {
         points[p] = IntPoint2{ p, numPoints - p };
      }
      points.back() = points.front();
      TestIntKdTree<2>(points);
   }
}


TEST(KdTreeTest, FindInRange2DFromExample)
{
   // See https://en.wikipedia.org/wiki/K-d_tree
   const std::array<IntPoint2, 6> points{ IntPoint2{2, 3}, IntPoint2{5, 4}, IntPoint2{9, 6}, IntPoint2{4, 7}, IntPoint2{8, 1}, IntPoint2{7, 2} };
   TestIntKdTree<2>(points);

   auto tree = KdTree<int, 2>::Create(points);

   auto searchRange = BoundingBox<int, 2>::CreateFromList(std::vector<IntPoint2>{ { 0, 0 }, { 6, 5 } });
   auto found = tree->FindInRange(searchRange);
   str::sort(found);
   ASSERT_TRUE(str::equal(std::vector<KdTreePosition> { 0, 1 }, found));

   searchRange = BoundingBox<int, 2>::CreateFromList(std::vector<IntPoint2>{ { 0, 5 }, { 10, 10 } });
    found = tree->FindInRange(searchRange);
   str::sort(found);
   ASSERT_TRUE(str::equal(std::vector<KdTreePosition> { 2, 3 }, found));

   searchRange = BoundingBox<int, 2>::CreateFromList(std::vector<IntPoint2>{ { 6, 0 }, { 10, 4 } });
   found = tree->FindInRange(searchRange);
   str::sort(found);
   ASSERT_TRUE(str::equal(std::vector<KdTreePosition> { 4,5 }, found));

   searchRange = BoundingBox<int, 2>::CreateFromList(std::vector<IntPoint2>{ { 0, 0  }, { 10, 3 } });
   found = tree->FindInRange(searchRange);
   str::sort(found);
   ASSERT_TRUE(str::equal(std::vector<KdTreePosition> { 0, 4, 5 }, found));

   searchRange = BoundingBox<int, 2>::CreateFromList(std::vector<IntPoint2>{ { 0, 0  }, { 10, 4 } });
   found = tree->FindInRange(searchRange);
   str::sort(found);
   ASSERT_TRUE(str::equal(std::vector<KdTreePosition> { 0, 1, 4, 5 }, found));
}