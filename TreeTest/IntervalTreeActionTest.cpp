#include <gtest/gtest.h>

#include "IntervalTree.h"
#include "IntervalTreeAction.h"

using namespace IntervalTree;

TEST(IntervalTreeActionTest, CountRoot)
{
   IndexTree<2> tree;
   ActionCount<2> count;
   tree.foreachLeaf(count);
   ASSERT_EQ(count(), 1);
}


TEST(IntervalTreeActionTest, ActionMaxLevel)
{
   IndexTree<1> tree;
   ActionMaxLevel<1> a;
   tree.foreachLeaf(a);
   ASSERT_EQ(a.MaxLevel, 0);
}

