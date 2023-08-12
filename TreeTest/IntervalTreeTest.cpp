#include <gtest/gtest.h>

#include "IntervalTree.h"

using namespace IntervalTree;

TEST(IndexTreeTest, DefaultConsturctor)
{
   IndexTree<2> tree;
   const auto* root = tree.getRoot();
   ASSERT_EQ(root->getLevel(), 0);
}

