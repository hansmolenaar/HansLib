#include <gtest/gtest.h>

#include "IndexTreeToSimplices2.h"
#include "IntervalTreeBalance.h"
#include "IntervalTreeRefinePredicate.h"
#include "IntervalTreeVtk.h"
#include "Paraview.h"
#include "ProjectToVtk.h"
#include "Triangle.h"
#include "UniqueHashedPointCollection.h"

using namespace IntervalTree;
using namespace IndexTreeToSimplices2;
using namespace MeshGeneration;

namespace
{
   void testOrientation(const Triangles triangles)
   {
      for (const auto& triangle : triangles)
      {
         const double area = Triangle::AreaSigned(PointUtils::toPoint(triangle[0]), PointUtils::toPoint(triangle[1]), PointUtils::toPoint(triangle[2]));
         ASSERT_GT(area, 0.0);
      }
   }
}

TEST(IndexTreeToSimplices2Test, RootToVtk)
{
   IndexTree<2> tree;
   const auto& root = tree.getRoot();

   const auto triangles = IndexTreeToSimplices2::Create(tree);
   ASSERT_EQ(2, triangles.size());
   testOrientation(triangles);

   ProjectToVtk p2v("IndexTreeToSimplices2Test_RootToVtk");
   IndexTreeToSimplices2::toVtkData(p2v, triangles);
   const auto* vtkData = p2v.get().front();
   ASSERT_EQ(2, vtkData->getNumCells());
   ASSERT_EQ(4, vtkData->getNumNodes());
   //Paraview::WriteList(p2v.get());
}

TEST(IndexTreeToSimplices2Test, Level1ToVtk)
{
   IndexTree<2> tree;
   const auto& root = tree.getRoot();
   RefineToMaxLevel<IndexTreeToSimplices2::GeometryDimension> doRefine1{ 1 };
   tree.refineLeaves(doRefine1);

   const auto triangles = IndexTreeToSimplices2::Create(tree);
   ASSERT_EQ(8, triangles.size());
   testOrientation(triangles);

   ProjectToVtk p2v("IndexTreeToSimplices2Test_Level1ToVtk");
   IndexTreeToSimplices2::toVtkData(p2v, triangles);
   const auto* vtkData = p2v.get().front();
   ASSERT_EQ(8, vtkData->getNumCells());
   ASSERT_EQ(9, vtkData->getNumNodes());
   //Paraview::WriteList(p2v.get());
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

   ProjectToVtk p2v("IndexTreeToSimplices2Test_RefinedToVtk_1");
   IndexTreeToSimplices2::toVtkData(p2v, triangles);
   Paraview::WriteList(p2v.get());

   const auto vtkData = p2v.get().front();
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
}

