#include <gtest/gtest.h>

#include "IndexTreeToSimplices2.h"
#include "IntervalTreeRefinePredicate.h"
#include "Paraview.h"

using namespace IntervalTree;

TEST(IndexTreeToSimplices2Test, RootToVtk)
{
   IndexTree<2> tree;
   const auto& root = tree.getRoot();

   const auto triangles = IndexTreeToSimplices2::Create(tree);
   ASSERT_EQ(2, triangles.size());

   const auto vtkData = IndexTreeToSimplices2::ToVtkData(triangles);
   ASSERT_EQ(2, vtkData->getNumCells());
   ASSERT_EQ(4, vtkData->getNumNodes());
   //Paraview::Write("IndexTreeToSimplices2Test_RootToVtk", *vtkData);
}


TEST(IndexTreeToSimplices2Test, Level1ToVtk)
{
   IndexTree<2> tree;
   const auto& root = tree.getRoot();
   RefineToMaxLevel<IndexTreeToSimplices2::GeometryDimension> doRefine1{ 1 };
   tree.refineLeaves(doRefine1);

   const auto triangles = IndexTreeToSimplices2::Create(tree);
   ASSERT_EQ(8, triangles.size());

   const auto vtkData = IndexTreeToSimplices2::ToVtkData(triangles);
   ASSERT_EQ(8, vtkData->getNumCells());
   ASSERT_EQ(9, vtkData->getNumNodes());
   //Paraview::Write("IndexTreeToSimplices2Test_Level1ToVtk", *vtkData);
}

