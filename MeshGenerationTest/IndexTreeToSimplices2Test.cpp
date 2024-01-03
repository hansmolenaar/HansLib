#include <gtest/gtest.h>

#include "IndexTreeToSimplices2.h"
#include "Paraview.h"

using namespace IntervalTree;

TEST(IndexTreeToSimplices2Test, RootToVtk)
{
   IndexTree<2> tree;
   const auto& root = tree.getRoot();

   const auto triangles = IndexTreeToSimplices2::Create(tree);
   ASSERT_EQ(2, triangles.size());

   const auto vtkData = IndexTreeToSimplices2::ToVtkData(triangles);
   Paraview::Write("IndexTreeToSimplices2Test_RootToVtk", *vtkData);
}

