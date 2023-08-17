#include <gtest/gtest.h>

#include "IntervalTree.h"
#include "IntervalTreeRefinePredicate.h"
#include "IntervalTreeAction.h"

using namespace IntervalTree;

TEST(IndexTreeTest, DefaultConsturctor)
{
   IndexTree<2> tree;
   const auto& root = tree.getRoot();
   ASSERT_EQ(root.getLevel(), 0);
   ASSERT_EQ(root.toString(), "((0, 1), (0, 1))");

   const auto& statistics = tree.getStatistics();
   const Statistics expect{ 1, {1} };
   ASSERT_EQ(statistics, expect);
}


TEST(IndexTreeTest, RefineOneLevel)
{
   IndexTree<1> tree;
   RefineToMaxLevel<1> doRefine2{ 2 };

   ActionCount<1> action;
   tree.foreachLeaf(action);
   ASSERT_EQ(action(), 1);
 
   int numRefined = tree.refineLeaves(doRefine2);
   ASSERT_EQ(numRefined, 1);
   action.reset();
   tree.foreachLeaf(action);
   ASSERT_EQ(action(), 2);

   Statistics expect{ 3, { 0, 2} };
   ASSERT_EQ(tree.getStatistics(), expect);

   numRefined = tree.refineLeaves(doRefine2);
   ASSERT_EQ(numRefined, 2);
   action.reset();
   tree.foreachLeaf(action);
   ASSERT_EQ(action(), 4);

    expect = { 7, { 0, 0, 4} };
   ASSERT_EQ(tree.getStatistics(), expect);
}


TEST(IndexTreeTest, refineUntilReady)
{
   IndexTree<2> tree;
   RefineToMaxLevel<2> doRefine2{ 2 };

   tree.refineUntilReady(doRefine2);
   ActionCount<2> action;
   tree.foreachLeaf(action);
   ASSERT_EQ(action(), 16);
}