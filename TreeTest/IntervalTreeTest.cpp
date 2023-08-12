#include <gtest/gtest.h>

#include "IntervalTree.h"

using namespace IntervalTree;

TEST(IndexTreeTest, DefaultConsturctor)
{
   IndexTree<2> tree;
   const auto* root = tree.getRoot();
   ASSERT_EQ(root->getLevel(), 0);
   ASSERT_EQ(root->toString(), "((0, 1), (0, 1))");
}

