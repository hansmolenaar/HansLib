#include <gtest/gtest.h>

#include "IndexTreeToSimplices2.h"
#include "IntervalTreeRefinePredicate.h"
#include "Paraview.h"
#include "IntervalTreeBalance.h"
#include "IntervalTreeVtk.h"

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
   //Paraview::Write("IndexTreeToSimplices2Test_RefinedToVtk_1_base", *IntervalTree::GetVtkData(tree));
   const auto triangles = IndexTreeToSimplices2::Create(tree);
   std::set<std::pair<RatPoint2, RatPoint2>> directedEdges;
   for (const auto& triangle : triangles)
   {
      const auto edge0 = std::make_pair(triangle.at(0), triangle.at(1));
      const auto edge1 = std::make_pair(triangle.at(1), triangle.at(2));
      const auto edge2 = std::make_pair(triangle.at(2), triangle.at(0));
      const std::array<std::pair<RatPoint2, RatPoint2>, 3> edges{edge0, edge1, edge2};
      for (const auto& e : edges)
      {
         ASSERT_FALSE(directedEdges.contains(e));
         directedEdges.insert(e);
      }
   }

   const auto vtkData = IndexTreeToSimplices2::ToVtkData(triangles);
   ASSERT_EQ(90, vtkData->getNumCells());
   ASSERT_EQ(54, vtkData->getNumNodes());
   //Paraview::Write("IndexTreeToSimplices2Test_RefinedToVtk_1", *vtkData);
}

