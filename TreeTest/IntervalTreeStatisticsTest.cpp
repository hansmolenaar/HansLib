#include <gtest/gtest.h>

#include "IntervalTreeStatistics.h"
#include "IntervalTreeRefinePredicate.h"

using namespace IntervalTree;

TEST(IntervalTreeStatistics, Root)
{
   IndexTree<3> tree;

   const auto& statistics = GetStatistics<3>(tree);
   const Statistics expect{ 1, {1} };
   ASSERT_EQ(statistics, expect);
}


TEST(IntervalTreeStatistics, Level1)
{
   IndexTree<3> tree;
   RefineToMaxLevel<3> doRefine{ 2 };
   tree.refineLeaves(doRefine);

   const auto& statistics = GetStatistics<3>(tree);
   ASSERT_EQ(statistics.toString(), "9, {0, 8}");
}
