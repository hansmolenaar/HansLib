#include <gtest/gtest.h>

#include "IndexTreeToSimplices2.h"
#include "IntervalTreeRefinePredicate.h"
#include "Paraview.h"
#include "IntervalTreeBalance.h"

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


TEST(IndexTreeToSimplices2Test, RefinedToVtk_1)
{
   constexpr auto dim = IndexTreeToSimplices2::GeometryDimension;
   const  std::array<Rational, dim> point{ Rational{49, 100}, Rational{51, 100} };
   RefineIfContainsPoint<dim> refinePoint{ point };
   RefineToMaxLevel<dim> refineToLevel{ 4 };
   auto doRefine = [&refinePoint, &refineToLevel](const Index<dim>& indx) {return refinePoint(indx) && refineToLevel(indx); };

   IndexTree<dim> tree;
   tree.refineUntilReady(doRefine);
   IntervalTree::Balance(tree);

   const auto triangles = IndexTreeToSimplices2::Create(tree);
   //ASSERT_EQ(8, triangles.size());

   const auto vtkData = IndexTreeToSimplices2::ToVtkData(triangles);
   //ASSERT_EQ(8, vtkData->getNumCells());
   //ASSERT_EQ(9, vtkData->getNumNodes());
   Paraview::Write("IndexTreeToSimplices2Test_RefinedToVtk_1", *vtkData);
}

