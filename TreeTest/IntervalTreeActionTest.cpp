#include <gtest/gtest.h>

#include "IntervalTreeAction.h"

using namespace IntervalTree;

TEST(IntervalTreeActionTest, CountRoot)
{
   IndexTree<2> tree;
   ActionCount<2> count;
   tree.foreachLeaf(count);
   ASSERT_EQ(count(), 1);
}

