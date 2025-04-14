#include <gtest/gtest.h>

#include "CellQuality2.h"
#include "EdgeFlip.h"
#include "Manifold1Reconstruction.h"
#include "ManifoldId.h"
#include "MeshQuality.h"
#include "PointClose.h"
#include "TopologyDefines.h"
#include "Triangle.h"
#include "TrianglesNodes.h"
#include "UniquePointCollectionBinning.h"

using namespace MeshGeneration;
using namespace Topology;
using namespace Utilities;
using namespace Geometry;

namespace
{
   void checkTriangleArea(const TrianglesNodes& trianglesNodes, const IPointCollection<GeomType, GeomDim2>& points)
   {
      for (CellIndex cellIndex : trianglesNodes.getAllTriangles())
      {
         const auto& triangleNodes = trianglesNodes.getTriangleNodes(cellIndex);
         const auto area = Triangle::getAreaSigned(triangleNodes, points);
         ASSERT_GT(area, 0.0);
      }
   }
}


TEST(EdgeFlipTest, base)
{
   const PointClose<GeomType, GeomDim2> areClose;
   UniquePointCollectionBinning<GeomDim2> points(areClose, std::vector<Point2>{Point2{ -1, -1 }, Point2{ 1,1 }});
   const auto node0 = points.addIfNew(Point2{ -1,0 });
   const auto node1 = points.addIfNew(Point2{ 1,0 });
   const auto node2 = points.addIfNew(Point2{ 0,0.1 });
   const auto node3 = points.addIfNew(Point2{ 0, -0.1 });
   TrianglesNodes trianglesNodes;
   trianglesNodes.addTriangle(TriangleNodesOriented(node0, node1, node2));
   trianglesNodes.addTriangle(TriangleNodesOriented(node0, node3, node1));

   const Boundary1 reconstruction = Boundary1::createSingleCycleForTesting(std::vector<NodeIndex>{ node0, node3, node1, node2 });

   const Geometry::ManifoldId manifoldId(Topology::Edge, "Loop");
   const Manifold1Reconstruction manifoldReconstruction(manifoldId, reconstruction);
   const std::vector<const IManifoldReconstruction*> reconstructions{ &manifoldReconstruction };

   checkTriangleArea(trianglesNodes, points);
   auto cellQuality = CellQuality2::MinimumAngle;
   EdgeFlip edgeFlip(trianglesNodes, cellQuality, points, reconstructions);
   ASSERT_TRUE(edgeFlip.isFlippable(EdgeNodesSorted(node0, node1)));
   ASSERT_TRUE(edgeFlip.isFlippable(EdgeNodesSorted(node2, node3))); // does not exist
   ASSERT_FALSE(edgeFlip.isFlippable(EdgeNodesSorted(node0, node3)));

   const double qualityBefore = MeshQuality::getQuality2WholeMesh(trianglesNodes, points, cellQuality);
   ASSERT_TRUE(std::abs(qualityBefore - 0.095176552291659472) < 1.0e-6);

   const EdgeFlipStrategy strategy{ 0.15, 10 };
   const int numSweeps = edgeFlip.execute(strategy);
   ASSERT_EQ(numSweeps, 2);

   const double qualityAfter = MeshQuality::getQuality2WholeMesh(trianglesNodes, points, cellQuality);
   ASSERT_TRUE(std::abs(qualityAfter - 0.19035310458332150) < 1.0e-6);
}


TEST(EdgeFlipTest, threeCells)
{
   const PointClose<GeomType, GeomDim2> areClose;
   UniquePointCollectionBinning<GeomDim2> points(areClose, std::vector<Point2>{Point2{ -1, -1 }, Point2{ 2,2 }});
   const auto node0 = points.addIfNew(Point2{ 0,0 });
   const auto node1 = points.addIfNew(Point2{ 1,0 });
   const auto node2 = points.addIfNew(Point2{ 0,1 });
   const auto node3 = points.addIfNew(Point2{ 0.5, -0.01 });
   const auto node4 = points.addIfNew(Point2{ -0.02, 0.5 });
   TrianglesNodes trianglesNodes;
   trianglesNodes.addTriangle(TriangleNodesOriented(node0, node1, node2));
   trianglesNodes.addTriangle(TriangleNodesOriented(node0, node3, node1));
   trianglesNodes.addTriangle(TriangleNodesOriented(node0, node2, node4));

   const Boundary1 reconstruction = Boundary1::createSingleCycleForTesting(std::vector<NodeIndex>{ node0, node3, node1, node2, node4 });

   const Geometry::ManifoldId manifoldId(Topology::Edge, "Loop");
   const Manifold1Reconstruction manifoldReconstruction(manifoldId, reconstruction);
   const std::vector<const IManifoldReconstruction*> reconstructions{ &manifoldReconstruction };

   auto cellQuality = CellQuality2::MinimumAngle;
   EdgeFlip edgeFlip(trianglesNodes, cellQuality, points, reconstructions);
   ASSERT_TRUE(edgeFlip.isFlippable(EdgeNodesSorted(node0, node1)));
   ASSERT_TRUE(edgeFlip.isFlippable(EdgeNodesSorted(node0, node2)));
   ASSERT_FALSE(edgeFlip.isFlippable(EdgeNodesSorted(node0, node3)));

   const double qualityBefore = MeshQuality::getQuality2WholeMesh(trianglesNodes, points, cellQuality);
   ASSERT_TRUE(std::abs(qualityBefore - 0.019096047302913772) < 1.0e-6);

   checkTriangleArea(trianglesNodes, points);
   const EdgeFlipStrategy strategy{ 1.0, 10 };
   const int numSweeps = edgeFlip.execute(strategy);
   ASSERT_EQ(numSweeps, 2);

   const double qualityAfter = MeshQuality::getQuality2WholeMesh(trianglesNodes, points, cellQuality);
   ASSERT_TRUE(std::abs(qualityAfter - 0.31103853056520964) < 1.0e-6);
}

TEST(EdgeFlipTest, nonConvex)
{
   const PointClose<GeomType, GeomDim2> areClose;
   UniquePointCollectionBinning<GeomDim2> points(areClose, std::vector<Point2>{Point2{ -2, -2 }, Point2{ 2,2 }});
   const auto node0 = points.addIfNew(Point2{ 0,0 });
   const auto node1 = points.addIfNew(Point2{ 0,1 });
   const auto node2 = points.addIfNew(Point2{ -0.1, -0.1 });
   const auto node3 = points.addIfNew(Point2{ 0.2, 0.1 });
   TrianglesNodes trianglesNodes;
   trianglesNodes.addTriangle(TriangleNodesOriented(node0, node1, node2));
   trianglesNodes.addTriangle(TriangleNodesOriented(node0, node3, node1));

   const Boundary1 reconstruction = Boundary1::createSingleCycleForTesting(std::vector<NodeIndex>{ node0, node3, node1, node2 });

   const Geometry::ManifoldId manifoldId(Topology::Edge, "Loop");
   const Manifold1Reconstruction manifoldReconstruction(manifoldId, reconstruction);
   const std::vector<const IManifoldReconstruction*> reconstructions{ &manifoldReconstruction };

   auto cellQuality = CellQuality2::MinimumAngle;
   EdgeFlip edgeFlip(trianglesNodes, cellQuality, points, reconstructions);

   const double qualityBefore = MeshQuality::getQuality2WholeMesh(trianglesNodes, points, cellQuality);
   ASSERT_TRUE(std::abs(qualityBefore - 0.086573815128912157) < 1.0e-6);
   checkTriangleArea(trianglesNodes, points);

   const EdgeFlipStrategy strategy{ 0.15, 10 };
   const int numSweeps = edgeFlip.execute(strategy);
   ASSERT_EQ(numSweeps, 1);

   // Nothing has happened
   const double qualityAfter = MeshQuality::getQuality2WholeMesh(trianglesNodes, points, cellQuality);
   ASSERT_TRUE(std::abs(qualityAfter - 0.086573815128912157) < 1.0e-6);
}
