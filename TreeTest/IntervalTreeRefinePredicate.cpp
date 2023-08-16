#include <gtest/gtest.h>

#include "IntervalTreeRefinePredicate.h"

using namespace IntervalTree;

TEST(RefineToMaxLevel, RefineToMaxLevel)
{
   IndexTree<1> tree;
   RefineToMaxLevel<1> doRefine0{ 0 };
   RefineToMaxLevel<1> doRefine1{ 1 };
   ASSERT_FALSE(doRefine0(tree.getRoot()));
   ASSERT_TRUE(doRefine1(tree.getRoot()));
}

