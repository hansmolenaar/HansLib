#include <gtest/gtest.h>

#include "KdTreeTraversorPointsInRange.h"
#include "KdTree.h"

TEST(KdTreeTraversorPointsInRangeTest, TestDeterminOverlap1D)
{
   const std::vector<std::vector<int>> points{ {1}, {5} };
   const KdTreeTraversorPointsInRange<int, 1> traversor(BoundingBox<int, 1>::CreateFromList(points));

   ASSERT_EQ(KdTreeOverlap::NoOverlap, traversor.DeterminOverlap(IntPoint1{ -3 }, IntPoint1{ -1 }));
   ASSERT_EQ(KdTreeOverlap::NoOverlap, traversor.DeterminOverlap(IntPoint1{ 7 }, IntPoint1 { 8 }));
   ASSERT_EQ(KdTreeOverlap::Overlap, traversor.DeterminOverlap(IntPoint1{ -3 }, IntPoint1 { 1 }));
   ASSERT_EQ(KdTreeOverlap::Overlap, traversor.DeterminOverlap(IntPoint1{ -3 }, IntPoint1 { 5 }));
   ASSERT_EQ(KdTreeOverlap::Overlap, traversor.DeterminOverlap(IntPoint1{ 0 }, IntPoint1 { 1 }));
   ASSERT_EQ(KdTreeOverlap::Overlap, traversor.DeterminOverlap(IntPoint1{ 5 }, IntPoint1 { 6 }));
   ASSERT_EQ(KdTreeOverlap::Overlap, traversor.DeterminOverlap(IntPoint1{ -3 }, IntPoint1 { 8 }));
   ASSERT_EQ(KdTreeOverlap::Overlap, traversor.DeterminOverlap(IntPoint1{ 0 }, IntPoint1 { 5 }));
   ASSERT_EQ(KdTreeOverlap::Overlap, traversor.DeterminOverlap(IntPoint1{ 1 }, IntPoint1 { 8 }));
   ASSERT_EQ(KdTreeOverlap::Contains, traversor.DeterminOverlap(IntPoint1{ 1 }, IntPoint1 { 5 }));
   ASSERT_EQ(KdTreeOverlap::Contains, traversor.DeterminOverlap(IntPoint1{ 1 }, IntPoint1 { 4 }));
   ASSERT_EQ(KdTreeOverlap::Contains, traversor.DeterminOverlap(IntPoint1 { 2 }, IntPoint1 { 5 }));
   ASSERT_EQ(KdTreeOverlap::Contains, traversor.DeterminOverlap(IntPoint1 { 1 }, IntPoint1 { 5 }));
}
