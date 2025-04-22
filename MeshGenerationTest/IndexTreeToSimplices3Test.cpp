#include <gtest/gtest.h>

#include "Boundary1.h"
#include "IndexTreeToSimplices3.h"
#include "IntervalTreeBalance.h"
#include "IntervalTreeRefinePredicate.h"
#include "IntervalTreeVtk.h"
#include "ITopologicalAdjacency.h"
#include "Paraview.h"
#include "ProjectToVtk.h"
#include "ReferenceShapeCube.h"
#include "Tetrahedron.h"
#include "TetrahedronsNodes.h"
#include "TopologyDefines.h"
#include "UniqueHashedPointCollection.h"

using namespace IntervalTree;
using namespace IndexTreeToSimplices3;
using namespace Topology;
using namespace MeshGeneration;

namespace
{
   void testOrientation(const TetrahedronNodesOriented& tet, const IPointCollection3& points)
   {
      const double volume = Tetrahedron::getSignedVolume(points.getPoint(tet[0]), points.getPoint(tet[1]), points.getPoint(tet[2]), points.getPoint(tet[3]));
      ASSERT_GT(volume, 0.0);
   }

   void checkMesh(
      const TetrahedronsNodes& tets,
      const IPointCollection3& points)
   {
      for (auto cellId : tets.getAllTetrahedrons())
      {
         const auto tet = tets.getTetrahedronNodes(cellId);
         testOrientation(tet, points);
      }
   }
}

TEST(IndexTreeToSimplices3Test, RootToVtk)
{
   IndexTree<GeomDim3> tree;
   const auto& root = tree.getRoot();

   const auto tets = IndexTreeToSimplices3::Create(tree);
   ASSERT_EQ(ReferenceShapeCube::numTetsInStandardSplit, tets.size());
   TetrahedronsNodes allTets;
   UniqueHashedPointCollection<double, GeomDim3> points;
   for (const auto& tet : tets)
   {
      std::array<NodeIndex, Topology::NumNodesOnTetrahedron> nodes;
      for (size_t pos = 0; auto n : tet)
      {
         nodes[pos] = points.addIfNew(PointUtils::toPoint(n));
         ++pos;
      }
      allTets.addTetrahedron(TetrahedronNodesOriented(nodes));
   }
   checkMesh(allTets, points);

   ProjectToVtk toVtk("IndexTreeToSimplices3Test_RootToVtk");
   IndexTreeToSimplices3::cellsToVtkData(toVtk, tets);
   const auto* vtkMesh = toVtk.get().front();
   ASSERT_EQ(ReferenceShapeCube::numTetsInStandardSplit, vtkMesh->getNumCells());
   ASSERT_EQ(NumNodesOnCube, vtkMesh->getNumNodes());
   //Paraview::Paraview::WriteList(vtkData);
}


TEST(IndexTreeToSimplices3Test, Level1ToVtk)
{
   IndexTree<GeomDim3> tree;
   const auto& root = tree.getRoot();
   RefineToMaxLevel<GeomDim3> doRefine1{ 1 };
   tree.refineLeaves(doRefine1);

   const auto tets = IndexTreeToSimplices3::Create(tree);
   ASSERT_EQ(8 * ReferenceShapeCube::numTetsInStandardSplit, tets.size());

   TetrahedronsNodes allTets;
   UniqueHashedPointCollection<double, GeomDim3> points;
   for (const auto& tet : tets)
   {
      std::array<NodeIndex, Topology::NumNodesOnTetrahedron> nodes;
      for (size_t pos = 0; auto n : tet)
      {
         nodes[pos] = points.addIfNew(PointUtils::toPoint(n));
         ++pos;
      }
      allTets.addTetrahedron(TetrahedronNodesOriented(nodes));
   }
   checkMesh(allTets, points);

   ProjectToVtk toVtk("IndexTreeToSimplices3Test_Level1ToVtk");
   IndexTreeToSimplices3::cellsToVtkData(toVtk, tets);
   const auto* vtkMesh = toVtk.get().front();
   ASSERT_EQ(8 * ReferenceShapeCube::numTetsInStandardSplit, vtkMesh->getNumCells());
   ASSERT_EQ(27, vtkMesh->getNumNodes());
   Paraview::WriteList(toVtk.get());

   const auto boundaryFaces = allTets.getBoundaryFaces();
   ASSERT_EQ(boundaryFaces.getNumTriangles(), 48);
   const Boundary1 bnd = Boundary1::createFromBoundaryEdges(boundaryFaces);
   ASSERT_TRUE(bnd.empty());
}

TEST(IndexTreeToSimplices3Test, GetCubeFromIndex)
{
   const IndexTree<GeomDim3> tree;
   const auto cube = IndexTreeToSimplices3::getCubeFromIndex(tree.getRoot());
   const auto* adjacencyC2E = *ReferenceShapeCube::getInstance().getAdjacencies().getAdjacency(Topology::Corner, Topology::Edge);
   for (int edge = 0; edge < Topology::NumEdgesOnCube; ++edge)
   {
      const auto& edgePoints = adjacencyC2E->getConnectedLowers(edge);
      ASSERT_EQ(edgePoints.size(), 2);
      ASSERT_EQ(PointUtils::GetL1Distance(cube[edgePoints[0]], cube[edgePoints[1]]), 1);
   }
}

TEST(IndexTreeToSimplices3Test, GetRefinedEdges1)
{
   IndexTree<GeomDim3> tree;
   RefineToMaxLevel<GeomDim3> doRefine1{ 1 };
   tree.refineLeaves(doRefine1);
   const std::set<IndexTreeToSimplices3::TreeEdge> refinedEdges = IndexTreeToSimplices3::getRefinedEdges(tree);
   ASSERT_EQ(refinedEdges.size(), Topology::NumEdgesOnCube);
   for (const auto& edge : refinedEdges)
   {
      ASSERT_EQ(PointUtils::GetL1Distance(edge.point0, edge.point1), 1);
   }
}

#if false
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