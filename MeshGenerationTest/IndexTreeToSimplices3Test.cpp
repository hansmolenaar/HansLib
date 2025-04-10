#include <gtest/gtest.h>

#include "IndexTreeToSimplices3.h"
#include "IntervalTreeBalance.h"
#include "IntervalTreeRefinePredicate.h"
#include "IntervalTreeVtk.h"
#include "Paraview.h"
#include "ReferenceShapeCube.h"
#include "Tetrahedron.h"
#include "TopologyDefines.h"
#include "UniqueHashedPointCollection.h"

using namespace IntervalTree;
using namespace IndexTreeToSimplices3;
using namespace Topology;

namespace
{
   void testOrientation(const Tetrahedrons tets)
   {
      for (const auto& tet : tets)
      {
         const double volume = Tetrahedron::getSignedVolume(PointUtils::toPoint(tet[0]), PointUtils::toPoint(tet[1]), PointUtils::toPoint(tet[2]), PointUtils::toPoint(tet[3]));
         ASSERT_GT(volume, 0.0);
      }
   }
}

TEST(IndexTreeToSimplices3Test, RootToVtk)
{
   IndexTree<GeomDim3> tree;
   const auto& root = tree.getRoot();

   const auto tets = IndexTreeToSimplices3::Create(tree);
   ASSERT_EQ(ReferenceShapeCube::numTetsInStandardSplit, tets.size());
   testOrientation(tets);

   const auto vtkData = IndexTreeToSimplices3::ToVtkData(tets, { "IndexTreeToSimplices3Test_RootToVtk", "tree" });
   ASSERT_EQ(ReferenceShapeCube::numTetsInStandardSplit, vtkData->getNumCells());
   ASSERT_EQ(NumNodesOnCube, vtkData->getNumNodes());
   Paraview::Write(*vtkData);

   for (auto c = 0; c < 6; ++c)
   {
      const auto vtkDataCell = IndexTreeToSimplices3::ToVtkData(std::vector<std::array<RatPoint3, 4>>{tets[c]}, { "IndexTreeToSimplices3Test_RootToVtk_SingleCell" + std::to_string(c), "cell" + std::to_string(c) });
      Paraview::Write(*vtkDataCell);
   }
}

#if false
TEST(IndexTreeToSimplices3Test, Level1ToVtk)
{
   IndexTree<2> tree;
   const auto& root = tree.getRoot();
   RefineToMaxLevel<IndexTreeToSimplices2::GeometryDimension> doRefine1{ 1 };
   tree.refineLeaves(doRefine1);

   const auto triangles = IndexTreeToSimplices2::Create(tree);
   ASSERT_EQ(8, triangles.size());
   testOrientation(triangles);

   const auto vtkData = IndexTreeToSimplices2::ToVtkData(triangles, { "IndexTreeToSimplices3Test_Level1ToVtk", "tree" });
   ASSERT_EQ(8, vtkData->getNumCells());
   ASSERT_EQ(9, vtkData->getNumNodes());
   //Paraview::Write(*vtkData);
}

TEST(IndexTreeToSimplices3Test, RefinedToVtk_1)
{
   constexpr auto dim = IndexTreeToSimplices2::GeometryDimension;
   const  std::array<Rational, dim> point{ Rational{49, 100}, Rational{51, 100} };
   RefineIfContainsPoint<dim> refinePoint{ point };
   RefineToMaxLevel<dim> refineToLevel{ 4 };
   auto doRefine = [&refinePoint, &refineToLevel](const Index<dim>& indx) {return refinePoint(indx) && refineToLevel(indx); };

   IndexTree<dim> tree;
   tree.refineUntilReady(doRefine);
   IntervalTree::Balance(tree);
   //Paraview::Write("IndexTreeToSimplices3Test_RefinedToVtk_1_base", *IntervalTree::GetVtkData(tree));
   const auto triangles = IndexTreeToSimplices2::Create(tree);
   testOrientation(triangles);
   std::set<std::pair<RatPoint2, RatPoint2>> directedEdges;
   UniqueHashedPointCollection<Rational, IndexTreeToSimplices2::GeometryDimension>  toNodeIndex;
   std::map<std::pair<PointIndex, PointIndex>, int> sortedEdgeCount;
   for (const auto& triangle : triangles)
   {
      const auto edge0 = std::make_pair(triangle.at(0), triangle.at(1));
      const auto edge1 = std::make_pair(triangle.at(1), triangle.at(2));
      const auto edge2 = std::make_pair(triangle.at(2), triangle.at(0));
      const std::array<std::pair<RatPoint2, RatPoint2>, 3> edges{ edge0, edge1, edge2 };
      for (const auto& e : edges)
      {
         ASSERT_FALSE(directedEdges.contains(e));
         directedEdges.insert(e);

         const auto c1 = toNodeIndex.addIfNew(e.first);
         const auto c2 = toNodeIndex.addIfNew(e.second);
         const auto sortedEdge = std::make_pair(std::min(c1, c2), std::max(c1, c2));
         sortedEdgeCount[sortedEdge] += 1;
      }
   }

   const auto vtkData = IndexTreeToSimplices2::ToVtkData(triangles, { "IndexTreeToSimplices3Test_RefinedToVtk_1" , "tree" });
   ASSERT_EQ(90, vtkData->getNumCells());
   ASSERT_EQ(54, vtkData->getNumNodes());

   Rational length(0, 1);
   for (const auto& itr : sortedEdgeCount)
   {
      if (itr.second == 1)
      {
         const auto c1 = toNodeIndex.getPoint(itr.first.first);
         const auto c2 = toNodeIndex.getPoint(itr.first.second);
         Rational dif;
         if (c1.at(0) == c2.at(0))
         {
            dif = c1.at(1) - c2.at(1);
         }
         else
         {
            ASSERT_EQ(c1.at(1), c2.at(1));
            dif = c1.at(0) - c2.at(0);
         }
         length += std::abs(dif);
      }
      else
      {
         ASSERT_EQ(2, itr.second);
      }
   }
   ASSERT_EQ(4, length);
   //Paraview::Write(*vtkData);
}

#endif