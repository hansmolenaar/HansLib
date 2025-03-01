#include <gtest/gtest.h>

#include "IntervalTree.h"
#include "IntervalTreeAction.h"
#include "IntervalTreeBalance.h"
#include "IntervalTreeRefinePredicate.h"
#include "IntervalTreeStatistics.h"
#include "IntervalTreeVtk.h"
#include "Paraview.h"
#include "Point.h"

using namespace IntervalTree;


TEST(IndexTreeVtkTest, Level1ToVtk_dim1)
{
   constexpr unsigned dim = 1;
   IndexTree<dim> tree;
   RefineToMaxLevel<dim> doRefine{ 1 };
   tree.refineUntilReady(doRefine);
   const auto data = GetVtkData(tree);
   ASSERT_EQ(data->getNumNodes(), 3);
   ASSERT_EQ(data->getNumCells(), 2);
   ASSERT_EQ(data->getNumCellData(), 0);
}


TEST(IndexTreeVtkTest, Level1ToVtk_dim2)
{
   constexpr unsigned dim = 2;
   IndexTree<dim> tree;
   RefineToMaxLevel<dim> doRefine{ 1 };
   tree.refineUntilReady(doRefine);
   const auto data = GetVtkData(tree);
   ASSERT_EQ(data->getNumNodes(), 9);
   ASSERT_EQ(data->getNumCells(), 4);
   ASSERT_EQ(data->getNumCellData(), 0);
}


TEST(IndexTreeVtkTest, Level1ToVtk_dim3)
{
   constexpr unsigned dim = 3;
   IndexTree<dim> tree;
   RefineToMaxLevel<dim> doRefine{ 1 };
   tree.refineUntilReady(doRefine);
   const auto data = GetVtkData(tree);
   ASSERT_EQ(data->getNumNodes(), 27);
   ASSERT_EQ(data->getNumCells(), 8);
   ASSERT_EQ(data->getNumCellData(), 0);
}
