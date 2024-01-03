#include <gtest/gtest.h>

#include "IntervalTree.h"
#include "IntervalTreeRefinePredicate.h"
#include "IntervalTreeStatistics.h"
#include "IntervalTreeAction.h"
#include "IntervalTreeVtk.h"
#include "IntervalTreeBalance.h"
#include "Paraview.h"
#include "Point.h"

using namespace IntervalTree;

TEST(IndexTreeTest, DefaultConsturctor)
{
   IndexTree<2> tree;
   const auto& root = tree.getRoot();
   ASSERT_EQ(root.getLevel(), 0);
   ASSERT_EQ(root.toString(), "((0, 1), (0, 1))");

   const auto& statistics = GetStatistics<2>(tree);
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
   ASSERT_EQ(GetStatistics(tree), expect);

   numRefined = tree.refineLeaves(doRefine2);
   ASSERT_EQ(numRefined, 2);
   action.reset();
   tree.foreachLeaf(action);
   ASSERT_EQ(action(), 4);

   expect = { 7, { 0, 0, 4} };
   ASSERT_EQ(GetStatistics(tree), expect);
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

TEST(IndexTreeTest, Level0ToVtk)
{
   const IndexTree<2> tree;
   const auto data = GetVtkData(tree);
   ASSERT_EQ(data->getNumNodes(), 4);
   ASSERT_EQ(data->getNumCells(), 1);
   ASSERT_EQ(data->getNumCellData(), 0);
   //Paraview::Write("IndexTreeTest_Level0ToVtk", *data);
}


TEST(IndexTreeTest, Level1ToVtk)
{
   IndexTree<2> tree;
   RefineToMaxLevel<2> doRefine{ 1 };
   tree.refineUntilReady(doRefine);
   const auto data = GetVtkData(tree);
   ASSERT_EQ(data->getNumNodes(), 9);
   ASSERT_EQ(data->getNumCells(), 4);
   ASSERT_EQ(data->getNumCellData(), 0);
   //Paraview::Write("IndexTreeTest_Level1ToVtk", *data);
}



TEST(IndexTreeTest, Level2ToVtk)
{
   IndexTree<1> tree;
   RefineToMaxLevel<1> doRefine{ 2 };
   tree.refineUntilReady(doRefine);
   const auto data = GetVtkData(tree);
   ASSERT_EQ(data->getNumNodes(), 5);
   ASSERT_EQ(data->getNumCells(), 4);
   ASSERT_EQ(data->getNumCellData(), 0);
   //Paraview::Write("IndexTreeTest_Level2ToVtk", *data);
}


TEST(IndexTreeTest, CubeToVtk)
{
   IndexTree<3> tree;
   const auto data = GetVtkData(tree);
   ASSERT_EQ(data->getNumNodes(), 8);
   ASSERT_EQ(data->getNumCells(), 1);
   ASSERT_EQ(data->getNumCellData(), 0);
   //Paraview::Write("IndexTreeTest_CubeToVtk", *data);
   const auto str = tree.getRoot().toString();
   ASSERT_EQ(str, "((0, 1), (0, 1), (0, 1))");
}
TEST(IndexTreeTest, Contains)
{
   IndexTree<2> tree;
   const auto& root = tree.getRoot();

   ASSERT_TRUE(tree.contains(root.getKey()));
   const auto kids = root.refine();

   ASSERT_FALSE(tree.contains(kids[0]));

   RefineToMaxLevel<2> refineToLevel{ 5 };
   tree.refineLeaves(refineToLevel);
   // Kid is a leaf
   ASSERT_TRUE(tree.contains(kids[0]));

   tree.refineLeaves(refineToLevel);
   // Kid is no longer a leaf
   ASSERT_TRUE(tree.contains(kids[0]));
}


TEST(IndexTreeTest, Get)
{
   IndexTree<3> tree;
   const auto& root = tree.getRoot();

   const auto retval = tree.get(root.getKey());
   ASSERT_TRUE(std::get<0>(retval));
   ASSERT_TRUE(std::get<1>(retval).isRoot());

   const Index<3>::Key key = { 1, 2, 1 };
   ASSERT_FALSE(std::get<0>(tree.get(key)));
}

TEST(IndexTreeTest, GetExistingSelfOrAncestor)
{
   RefineToMaxLevel<2> refineToLevel{ 5 };

   IndexTree<2> tree;

   const auto& root = tree.getRoot();
   ASSERT_TRUE(tree.contains(root.getKey()));

   const Index<2>::Key key{ 3, 6 };
   const auto& indexAtLevel0 = tree.getExistingSelfOrAncestor(key);
   Index<2>::Key expect{ 0, 0 };
   ASSERT_EQ(indexAtLevel0.getKey(), expect);

   tree.refineLeaves(refineToLevel);
   const auto& indexAtLevel1 = tree.getExistingSelfOrAncestor(key);
   expect = { 1, 2 };
   ASSERT_EQ(indexAtLevel1.getKey(), expect);

   tree.refineLeaves(refineToLevel);
   const auto& indexAtLevel2 = tree.getExistingSelfOrAncestor(key);
   ASSERT_EQ(indexAtLevel2.getKey(), key);
}


TEST(IndexTreeTest, GetExistingSelfOrAncestor2)
{
   IndexTree<3> tree;
   Index<3>::Key key = { 3,4,5 };
   const auto& root = tree.getExistingSelfOrAncestor(key);
   ASSERT_TRUE(root.isRoot());
}


TEST(IndexTreeTest, IsRefined1)
{
   IndexTree<1> tree;
   ASSERT_TRUE(tree.isLeaf(tree.getRoot()));

   RefineToMaxLevel<1> doRefine1{ 1 };
    tree.refineLeaves(doRefine1);

   ASSERT_FALSE(tree.isLeaf(tree.getRoot()));
}