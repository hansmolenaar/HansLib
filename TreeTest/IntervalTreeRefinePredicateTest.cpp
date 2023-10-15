#include <gtest/gtest.h>

#include "IntervalTreeRefinePredicate.h"

using namespace IntervalTree;

TEST(IntervalTreeRefinePredicateTest, RefineToMaxLevel)
{
   IndexTree<1> tree;
   RefineToMaxLevel<1> doRefine0{ 0 };
   RefineToMaxLevel<1> doRefine1{ 1 };
   ASSERT_FALSE(doRefine0(tree.getRoot()));
   ASSERT_TRUE(doRefine1(tree.getRoot()));
}


TEST(IntervalTreeRefinePredicateTest, RefineIfOverlaps)
{
   const auto bbRegion = BoundingBox<double, 1>::Create(std::array<double, 1>{10.5});
   const auto bbTree = BoundingBox<double, 1>::CreateFrom2Points(Point1{ 10.0 }, Point1{ 12.0 });
   const RefineIfOverlaps<1> doRefine{ bbRegion, bbTree };

   IndexFactory<1> factory;
   const auto* root = factory.getRoot();
   ASSERT_TRUE(doRefine(*root));

   const auto kids = root->refine();
   const auto* kid0 = factory.addIfNew(kids[0]);
   const auto* kid1 = factory.addIfNew(kids[1]);
   ASSERT_TRUE(doRefine(*kid0));
   ASSERT_FALSE(doRefine(*kid1));
}