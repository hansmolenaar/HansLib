#include <gtest/gtest.h>

#include "Ball.h"
#include "Ball2AsRegion.h"
#include "EdgeFlip.h"
#include "IndexTreeScaled.h"
#include "InitialBoundingboxGenerator.h"
#include "IntervalTreeIndexFactory.h"
#include "IRegionManifoldsTest.h"
#include "Manifold0.h"
#include "Manifold1Reconstruction.h"
#include "Mesh2.h"
#include "MeshGeneration2.h"
#include "MeshGenerationUtils.h"
#include "MeshingSettingsStandard.h"
#include "MeshStatistics.h"
#include "Nibble.h"
#include "Paraview.h"
#include "PointClose.h"
#include "Polygon2AsRegion.h"
#include "RefinementPredicates.h"
#include "Single.h"
#include "Triangle.h"
#include "UniquePointCollectionBinning.h"

#include <numbers>
#include <set>

using namespace MeshGeneration;
using namespace MeshGeneration2;
using namespace IntervalTree;
using namespace Geometry;
using namespace Utilities;
using namespace Topology;

namespace
{
   std::vector<Point2> generateRegularPolygon(size_t numCorners, double angle)
   {
      std::vector<Point2> result;
      double delAngle = 2 * std::numbers::pi / numCorners;
      for (size_t n = 0; n < numCorners; ++n)
      {
         result.push_back(Point2{ std::cos(angle), std::sin(angle) });
         angle += delAngle;
      }
      return result;
   }

   void checkTriangleArea(const TrianglesNodes& trianglesNodes, const IPointCollection<GeomType, GeomDim2>& points)
   {
      for (CellIndex cellIndex : trianglesNodes.getAllTriangles())
      {
         const auto& triangleNodes = trianglesNodes.getTriangleNodes(cellIndex);
         const auto area = Triangle::getAreaSigned(triangleNodes, points);
         ASSERT_GT(area, 0.0);
      }
   }

   std::vector<EdgeNodesDirected> checkEdgeOrientations(const TrianglesNodes& trianglesNodes)
   {
      std::set<EdgeNodesDirected> uniqueEdges;
      for (CellIndex triangleId : trianglesNodes.getAllTriangles())
      {
         const auto triangle = trianglesNodes.getTriangleNodes(triangleId);
         for (const auto& edge : triangle.getEdges())
         {
            const auto retval = uniqueEdges.insert(edge);
            if (!retval.second)
            {
               throw MyException("checkEdgeOrientations overconnected edge");
            }
         }
      }

      std::vector<EdgeNodesDirected> result;
      for (const auto& edge : uniqueEdges)
      {
         if (!uniqueEdges.contains(EdgeNodesDirected(edge[1], edge[0])))
         {
            result.push_back(edge);
         }
      }
      return result;
   }

   void checkBoundaryEdgesInReconstruction(
      const std::vector<EdgeNodesDirected>& boundaryEdges,
      const std::vector<const MeshGeneration::IManifoldReconstruction*>& reconstructions)
   {
      std::vector< const Manifold1Reconstruction*> manifolds1(reconstructions.size());
      str::transform(reconstructions, manifolds1.begin(), [](const auto* ptr) {return dynamic_cast<const Manifold1Reconstruction*>(ptr); });
      std::erase(manifolds1, nullptr);

      for (const auto& edge : boundaryEdges)
      {
         const EdgeNodesSorted edgeSorted(edge[0], edge[1]);
         ASSERT_TRUE(str::any_of(manifolds1, [&edgeSorted](const  Manifold1Reconstruction* mptr) {return mptr->contains(edgeSorted); }));
      }
   }

   void checkMesh(
      const IGeometryRegion<double, GeomDim2>& region,
      const TrianglesNodes& triangles,
      const std::vector<const MeshGeneration::IManifoldReconstruction*>& reconstructions,
      const IPointCollection2& points,
      IMeshingSettings2& settings)
   {
      IRegionManifoldsTestInterface(region.getManifolds(), settings.getGeometryPredicate());
      checkTriangleArea(triangles, points);
      const bool succes = MeshGeneration2::checkReconstructions(region.getManifolds(), reconstructions, settings.getLogger());
      ASSERT_TRUE(succes);

      const auto boundaryEdges = checkEdgeOrientations(triangles);
      checkBoundaryEdgesInReconstruction(boundaryEdges, reconstructions);

   }
}

TEST(MeshGeneration2Test, Ball)
{
   const Ball<GeomType, GeomDim2> ball(Point2{ 0.5, 0.5 }, 0.5);
   const Ball2AsRegion<GeomType> ballAsRegion(ball, "MeshGeneration2Test_Bal");
   MeshingSettingsStandard<2> settings(5, 2.0);
   IRegionManifoldsTestInterface(ballAsRegion.getManifolds(), settings.getGeometryPredicate());
   MeshGeneration2::Mesh2 mesh;
   mesh.setBaseTriangles(MeshGeneration2::GenerateBaseTriangulation(ballAsRegion, settings));
   const auto vtkData = IndexTreeToSimplices2::ToVtkData(mesh.getBaseTriangles(), { "MeshGeneration2Test_Ball" , "triangles" });
   ASSERT_EQ(1016, vtkData->getNumCells());
   //Paraview::Write(*vtkData);
}


TEST(MeshGeneration2Test, SingleTriangleToWorld)
{
   Logger logger;
   IndexTreeToSimplices2::Triangles baseTriangles;
   const RatPoint2 rp0(Rational(0, 1), Rational(0, 1));
   const RatPoint2 rp1(Rational(1, 1), Rational(0, 1));
   const RatPoint2 rp2(Rational(0, 1), Rational(1, 1));
   baseTriangles.push_back({ rp0, rp1, rp2 });
   const auto bb = BoundingBox<GeomType, GeomDim2>::CreateFrom2Points(Point2{ 1,1 }, Point2{ 2,3 });
   const PointClose<GeomType, GeomDim2> areClose;

   Mesh2 mesh;
   MeshGeneration2::BaseTriangulationToWorld(baseTriangles, areClose, bb, mesh.createPoints(), mesh.getTriangles(), logger);
   const auto& pointGeometry = mesh.getPoints();
   ASSERT_EQ(pointGeometry.getNumPoints(), 3);
   ASSERT_TRUE(areClose(pointGeometry.getPoint(0), Point2{ 1,1 }));
   ASSERT_TRUE(areClose(pointGeometry.getPoint(1), Point2{ 2,1 }));
   ASSERT_TRUE(areClose(pointGeometry.getPoint(2), Point2{ 1,3 }));
   checkTriangleArea(mesh.getTriangles(), mesh.getPoints());

   ASSERT_EQ(mesh.getTriangles().getAllTriangles().size(), 1);

   //const auto vtkData = MeshGeneration2::ToVtkData(*triangleNodes, *pointGeometry);
   //Paraview::Write("MeshGeneration2Test_SingleTriangleToWorld", *vtkData);
}


TEST(MeshGeneration2Test, Ball2)
{
   const std::string project = "MeshGeneration2Test_Ball2";
   const Ball<GeomType, GeomDim2> ball(Point2{ 1.5, 2.5 }, 3);
   const Ball2AsRegion<GeomType> ballAsRegion(ball, project);
   MeshingSettingsStandard<2> settings(4, 1.25);
   IRegionManifoldsTestInterface(ballAsRegion.getManifolds(), settings.getGeometryPredicate());
   const auto bbInitial = settings.getInitialBb(ballAsRegion);
   Mesh2 mesh;
   mesh.setBaseTriangles(MeshGeneration2::GenerateBaseTriangulation(ballAsRegion, settings));

   MeshGeneration2::BaseTriangulationToWorld(mesh.getBaseTriangles(), settings.getGeometryPredicate(), bbInitial, mesh.createPoints(), mesh.getTriangles(), settings.getLogger());

   const auto stats = MeshStatistics::Create2(mesh.getTriangles(), mesh.getPoints(), settings.getCellQuality());
   checkTriangleArea(mesh.getTriangles(), mesh.getPoints());
   const MeshStatistics expect{ 281, 504, 0.75 };
   ASSERT_EQ(stats, expect);

   const auto vtkData = MeshGeneration2::trianglesToVtkData(mesh, { project, "mesh" });
   ASSERT_EQ(504, vtkData->getNumCells());

   //Paraview::Write(*vtkData);
}


TEST(MeshGeneration2Test, Sphere2AndEdge)
{
   Logger logger;
   const Sphere<GeomType, GeomDim2> sphere(Point2{ 0, 0 }, 1);
   const auto bb = sphere.getBoundingBox();
   const Sphere2AsManifold1<GeomType> manifold(sphere);
   const PointClose<GeomType, GeomDim2> predicate;
   UniquePointCollectionBinning<GeomDim2> points(predicate, std::vector < Point2>{bb.getLower(), bb.getUpper()});
   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;
   TrianglesNodes trianglesNodes;
   const auto cellId = trianglesNodes.addTriangle(TriangleNodesOriented(points.addIfNew(Point2{ -1, -2 }), points.addIfNew(Point2{ 1, -2 }), points.addIfNew(Point2{ 0, -0.9 })));

   const NodeIndex node0 = points.addIfNew(Point2{ 0, -2 });
   const NodeIndex node1 = points.addIfNew(Point2{ 0, -0.9 });
   const EdgeNodesDirected edge{ node0, node1 };

   const bool anyNodeMoved = AddEdgeManifold1Intersections(manifold, edge, trianglesNodes, manifoldsAndNodes, points);
   checkTriangleArea(trianglesNodes, points);
   ASSERT_TRUE(anyNodeMoved);
   ASSERT_TRUE(predicate.samePoints(Point2{ 0,-1 }, points.getPoint(node1)));
}


TEST(MeshGeneration2Test, Sphere2AndEdge_immobilePoint)
{
   Logger logger;
   const Sphere<GeomType, GeomDim2> sphere(Point2{ 0, 0 }, 1);
   const auto bb = sphere.getBoundingBox();
   const Manifold0<GeomType, GeomDim2> pointManifold(Point2{ 0,-1 }, "MeshGeneration2Test_Sphere2AndEdge_immobilePoint");
   const Sphere2AsManifold1<GeomType> manifold(sphere);
   const PointClose<GeomType, GeomDim2> predicate;
   UniquePointCollectionBinning<GeomDim2> points(predicate, std::vector < Point2>{bb.getLower(), bb.getUpper()});
   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;
   TrianglesNodes trianglesNodes;
   const auto cellId = trianglesNodes.addTriangle(TriangleNodesOriented(points.addIfNew(Point2{ -1, -2 }), points.addIfNew(Point2{ 1, -2 }), points.addIfNew(Point2{ 0, -0.9 })));

   const NodeIndex node0 = points.addIfNew(Point2{ 0, -2 });
   const NodeIndex node1 = points.addIfNew(Point2{ 0, -0.9 });
   const NodeIndex node2 = points.addIfNew(Point2{ 0, -1 });
   const EdgeNodesDirected edge{ node0, node1 };
   manifoldsAndNodes.addNodeToManifold(node2, &pointManifold);
   checkTriangleArea(trianglesNodes, points);

   ASSERT_MYEXCEPTION_MESSAGE(AddEdgeManifold1Intersections(manifold, edge, trianglesNodes, manifoldsAndNodes, points), "UniquePointCollectionBinning<N>::movePoint point already exists");
}

TEST(MeshGeneration2Test, Sphere2AndEdge_NoIntersections)
{
   Logger logger;
   const Sphere<GeomType, GeomDim2> sphere(Point2{ 0, 0 }, 1);
   const auto bb = sphere.getBoundingBox();
   const Sphere2AsManifold1<GeomType> manifold(sphere);
   const PointClose<GeomType, GeomDim2> predicate;
   UniquePointCollectionBinning<GeomDim2> points(predicate, std::vector < Point2>{bb.getLower(), bb.getUpper()});
   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;
   TrianglesNodes trianglesNodes;

   const NodeIndex node0 = points.addIfNew(Point2{ -2, 2 });
   const NodeIndex node1 = points.addIfNew(Point2{ 2, 2 });
   const EdgeNodesDirected edge{ node0, node1 };

   const bool anyNodeMoved = AddEdgeManifold1Intersections(manifold, edge, trianglesNodes, manifoldsAndNodes, points);
   ASSERT_FALSE(anyNodeMoved);
   ASSERT_TRUE(manifoldsAndNodes.getNodesInManifold(&manifold).empty());
   checkTriangleArea(trianglesNodes, points);
}


TEST(MeshGeneration2Test, Sphere2AndEdge_EdgePointOnSphere)
{
   Logger logger;
   const Sphere<GeomType, GeomDim2> sphere(Point2{ 0, 0 }, 1);
   const auto bb = sphere.getBoundingBox();
   const Sphere2AsManifold1<GeomType> manifold(sphere);
   const PointClose<GeomType, GeomDim2> predicate;
   UniquePointCollectionBinning<GeomDim2> points(predicate, std::vector < Point2>{bb.getLower(), bb.getUpper()});
   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;
   TrianglesNodes trianglesNodes;

   const NodeIndex node0 = points.addIfNew(Point2{ 0, -1 });
   const NodeIndex node1 = points.addIfNew(Point2{ 0, -2 });
   const EdgeNodesDirected edge{ node0, node1 };

   const bool anyNodeMoved = AddEdgeManifold1Intersections(manifold, edge, trianglesNodes, manifoldsAndNodes, points);
   ASSERT_FALSE(anyNodeMoved);
   ASSERT_EQ(Utilities::Single(manifoldsAndNodes.getNodesInManifold(&manifold)), node0);
   checkTriangleArea(trianglesNodes, points);
}


TEST(MeshGeneration2Test, Sphere2AndEdge_EdgePointOnSphere2)
{
   Logger logger;
   const Sphere<GeomType, GeomDim2> sphere(Point2{ 0, 0 }, 1);
   const auto bb = sphere.getBoundingBox();
   const Sphere2AsManifold1<GeomType> manifold(sphere);
   const PointClose<GeomType, GeomDim2> predicate;
   UniquePointCollectionBinning<GeomDim2> points(predicate, std::vector < Point2>{bb.getLower(), bb.getUpper()});
   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;
   TrianglesNodes trianglesNodes;

   const NodeIndex node0 = points.addIfNew(Point2{ 0, -2 });
   const NodeIndex node1 = points.addIfNew(Point2{ 0, -1 });
   const EdgeNodesDirected edge{ node0, node1 };

   const bool anyNodeMoved = AddEdgeManifold1Intersections(manifold, edge, trianglesNodes, manifoldsAndNodes, points);
   ASSERT_FALSE(anyNodeMoved);
   ASSERT_EQ(Utilities::Single(manifoldsAndNodes.getNodesInManifold(&manifold)), node1);
   checkTriangleArea(trianglesNodes, points);
}


TEST(MeshGeneration2Test, Sphere2AndEdge_EdgePointBothOnSphere)
{
   Logger logger;
   const Sphere<GeomType, GeomDim2> sphere(Point2{ 0, 0 }, 1);
   const auto bb = sphere.getBoundingBox();
   const Sphere2AsManifold1<GeomType> manifold(sphere);
   const PointClose<GeomType, GeomDim2> predicate;
   UniquePointCollectionBinning<GeomDim2> points(predicate, std::vector < Point2>{bb.getLower(), bb.getUpper()});
   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;
   TrianglesNodes trianglesNodes;

   const NodeIndex node0 = points.addIfNew(Point2{ 0, 1 });
   const NodeIndex node1 = points.addIfNew(Point2{ 0, -1 });
   const EdgeNodesDirected edge{ node0, node1 };

   const bool anyNodeMoved = AddEdgeManifold1Intersections(manifold, edge, trianglesNodes, manifoldsAndNodes, points);
   ASSERT_FALSE(anyNodeMoved);
   ASSERT_EQ(manifoldsAndNodes.getNodesInManifold(&manifold).size(), 2);
   checkTriangleArea(trianglesNodes, points);
}


TEST(MeshGeneration2Test, Sphere2AndEdge_OneEdgePointOnSphereTwoIntersections)
{
   Logger logger;
   const Sphere<GeomType, GeomDim2> sphere(Point2{ 0, 0 }, 1);
   const auto bb = sphere.getBoundingBox();
   const Sphere2AsManifold1<GeomType> manifold(sphere);
   const PointClose<GeomType, GeomDim2> predicate;
   UniquePointCollectionBinning<GeomDim2> points(predicate, std::vector < Point2>{bb.getLower(), bb.getUpper()});
   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;
   TrianglesNodes trianglesNodes;

   const NodeIndex node0 = points.addIfNew(Point2{ 0, 1 });
   const NodeIndex node1 = points.addIfNew(Point2{ 0, -100 });
   const EdgeNodesDirected edge{ node0, node1 };

   const bool anyNodeMoved = AddEdgeManifold1Intersections(manifold, edge, trianglesNodes, manifoldsAndNodes, points);
   ASSERT_TRUE(anyNodeMoved);
   ASSERT_EQ(manifoldsAndNodes.getNodesInManifold(&manifold).size(), 2);
   ASSERT_TRUE(predicate.samePoints(points.getPoint(node1), Point2{ 0, -1 }));
   checkTriangleArea(trianglesNodes, points);
}


TEST(MeshGeneration2Test, Sphere2AndEdge_TwoIntersections)
{
   Logger logger;
   const Sphere<GeomType, GeomDim2> sphere(Point2{ 0, 0 }, 1);
   const auto bb = sphere.getBoundingBox();
   const Sphere2AsManifold1<GeomType> manifold(sphere);
   const PointClose<GeomType, GeomDim2> predicate;
   UniquePointCollectionBinning<GeomDim2> points(predicate, std::vector < Point2>{bb.getLower(), bb.getUpper()});
   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;
   TrianglesNodes trianglesNodes;

   const NodeIndex node0 = points.addIfNew(Point2{ 0, -2 });
   const NodeIndex node1 = points.addIfNew(Point2{ 0, 2 });
   const EdgeNodesDirected edge{ node0, node1 };

   const bool anyNodeMoved = AddEdgeManifold1Intersections(manifold, edge, trianglesNodes, manifoldsAndNodes, points);
   ASSERT_TRUE(anyNodeMoved);
   ASSERT_EQ(manifoldsAndNodes.getNodesInManifold(&manifold).size(), 2);
   ASSERT_TRUE(predicate.samePoints(points.getPoint(node0), Point2{ 0, -1 }));
   ASSERT_TRUE(predicate.samePoints(points.getPoint(node1), Point2{ 0, 1 }));
   checkTriangleArea(trianglesNodes, points);
}

TEST(MeshGeneration2Test, Sphere2_intersect_4)
{
   const std::string project = "MeshGeneration2Test_Sphere2_intersect_4";
   const Ball<GeomType, GeomDim2> ball(Point2{ 1.5, 2.5 }, 3);
   const Ball2AsRegion<GeomType> ballAsRegion(ball, project);
   MeshingSettingsStandard<2> settings(4, 1.25);
   Mesh2 mesh;
   mesh.setBaseTriangles(MeshGeneration2::GenerateBaseTriangulation(ballAsRegion, settings));

   const auto bbInitial = settings.getInitialBb(ballAsRegion);
   MeshGeneration2::BaseTriangulationToWorld(mesh.getBaseTriangles(), settings.getGeometryPredicate(), bbInitial, mesh.createPoints(), mesh.getTriangles(), settings.getLogger());

   auto stats = MeshStatistics::Create2(mesh.getTriangles(), mesh.getPoints(), settings.getCellQuality());
   MeshStatistics expect{ 281, 504, 0.75 };
   ASSERT_EQ(expect, stats);

   const auto manifoldPtr = dynamic_cast<const Geometry::IManifold1D2<GeomType>*>(Utilities::Single(ballAsRegion.getManifolds().getAllManifolds()));

   MeshGeneration2::AddManifold1Intersections(*manifoldPtr, mesh.getTriangles(), mesh.setManifoldsAndNodes(), mesh.getSetPoints(), settings.getLogger());

   stats = MeshStatistics::Create2(mesh.getTriangles(), mesh.getPoints(), settings.getCellQuality());
   expect = { 281, 504, 0.37443649960593806 };

   settings.getLogger().toFile("C:\\Users\\Hans\\Documents\\work\\logger.txt");
   ASSERT_EQ(expect, stats);

   const auto nodesOnManifold = mesh.getManifoldsAndNodes().getNodesInManifold(manifoldPtr);
   mesh.addReconstructions(MeshGeneration2::createReconstructions(ballAsRegion.getManifolds(), mesh.getTriangles(), mesh.getManifoldsAndNodes(), mesh.getPoints()));

   ASSERT_EQ(mesh.getReconstructions().size(), 1);
   const auto reconstruction = dynamic_cast<const Manifold1Reconstruction&>(*mesh.getReconstructions().front()).getReconstruction();
   ASSERT_TRUE(reconstruction.getSingletons().empty());
   ASSERT_TRUE(reconstruction.getPaths().empty());
   ASSERT_EQ(reconstruction.getCycles().size(), 1);
   ASSERT_EQ(Single(reconstruction.getCycles()).size(), 42);

   const auto vtkData = MeshGeneration2::trianglesToVtkData(mesh, { project, "mesh full" });
   const auto vtkDataManifold = MeshGeneration2::ToVtkData(reconstruction, mesh.getPoints(), { project, manifoldPtr->getName() });

   Paraview::Write(*vtkData);
   Paraview::Write(*(Utilities::Single(vtkDataManifold)));

   nibble(ballAsRegion, mesh.getReconstructions(), mesh.getTriangles(), mesh.getPoints(), settings.getLogger());
   ASSERT_EQ(mesh.getTriangles().getAllTriangles().size(), 250);
   ASSERT_EQ(mesh.getTriangles().getAllNodes().size(), 147);
   checkMesh(ballAsRegion, mesh.getTriangles(), mesh.getReconstructions(), mesh.getPoints(), settings);

   stats = MeshStatistics::Create2(mesh.getTriangles(), mesh.getPoints(), settings.getCellQuality());
   expect = { 147, 250, 0.37443649960593806 };
   ASSERT_EQ(expect, stats);

   const auto vtkDataNibbled = MeshGeneration2::trianglesToVtkData(mesh, { project, "mesh" });
   Paraview::Write(*vtkDataNibbled);
}

TEST(MeshGeneration2Test, Sphere2_intersect_3)
{
   const std::string project = "MeshGeneration2Test_Sphere2_intersect_3";
   const Ball<GeomType, GeomDim2> ball(Point2{ 1.5, 2.5 }, 3);
   const Ball2AsRegion<GeomType> ballAsRegion(ball, "MeshGeneration2Test_Sphere2_intersect_3");
   MeshingSettingsStandard<2> settings(3, 1.25);
   Mesh2 mesh;
   mesh.setBaseTriangles(MeshGeneration2::GenerateBaseTriangulation(ballAsRegion, settings));

   const auto bbInitial = settings.getInitialBb(ballAsRegion);
   MeshGeneration2::BaseTriangulationToWorld(mesh.getBaseTriangles(), settings.getGeometryPredicate(), bbInitial, mesh.createPoints(), mesh.getTriangles(), settings.getLogger());

   const auto manifoldPtr = dynamic_cast<const Geometry::IManifold1D2<GeomType>*>(Utilities::Single(ballAsRegion.getManifolds().getAllManifolds()));

   MeshGeneration2::AddManifold1Intersections(*manifoldPtr, mesh.getTriangles(), mesh.setManifoldsAndNodes(), mesh.getSetPoints(), settings.getLogger());

   auto stats = MeshStatistics::Create2(mesh.getTriangles(), mesh.getPoints(), settings.getCellQuality());
   MeshStatistics expect{ 81, 128, 00.30877886910687341 };
   ASSERT_EQ(expect, stats);

   const auto vtkData = MeshGeneration2::trianglesToVtkData(mesh, { project, "mesh full" });
   ASSERT_EQ(128, vtkData->getNumCells());
   const auto nodesOnManifold = mesh.getManifoldsAndNodes().getNodesInManifold(manifoldPtr);
   mesh.addReconstructions(MeshGeneration2::createReconstructions(ballAsRegion.getManifolds(), mesh.getTriangles(), mesh.getManifoldsAndNodes(), mesh.getPoints()));

   ASSERT_EQ(mesh.getReconstructions().size(), 1);
   const auto reconstruction = dynamic_cast<const Manifold1Reconstruction&>(*mesh.getReconstructions().front()).getReconstruction();

   ASSERT_TRUE(reconstruction.getSingletons().empty());
   ASSERT_TRUE(reconstruction.getPaths().empty());
   ASSERT_EQ(reconstruction.getCycles().size(), 1);
   ASSERT_EQ(Single(reconstruction.getCycles()).size(), 20);
   const auto vtkDataManifold = MeshGeneration2::ToVtkData(reconstruction, mesh.getPoints(), { project, manifoldPtr->getName() });

   Paraview::Write(*vtkData);
   Paraview::Write(*(Utilities::Single(vtkDataManifold)));

   ASSERT_EQ(mesh.getTriangles().getAllTriangles().size(), 128);
   ASSERT_EQ(mesh.getTriangles().getAllNodes().size(), 81);
   nibble(ballAsRegion, mesh.getReconstructions(), mesh.getTriangles(), mesh.getPoints(), settings.getLogger());
   ASSERT_EQ(mesh.getTriangles().getAllTriangles().size(), 60);
   ASSERT_EQ(mesh.getTriangles().getAllNodes().size(), 41);
   checkMesh(ballAsRegion, mesh.getTriangles(), mesh.getReconstructions(), mesh.getPoints(), settings);

   stats = MeshStatistics::Create2(mesh.getTriangles(), mesh.getPoints(), settings.getCellQuality());
   expect = { 41, 60, 0.55410066890581655 };
   ASSERT_EQ(expect, stats);

   const auto vtkDataNibbled = MeshGeneration2::trianglesToVtkData(mesh, { project, "mesh" });
   Paraview::Write(*vtkDataNibbled);
}

TEST(MeshGeneration2Test, Sphere2_intersect_5)
{
   const std::string project = "MeshGeneration2Test_Sphere2_intersect_5";
   const Ball<GeomType, GeomDim2> ball(Point2{ 1.5, 2.5 }, 3);
   const Ball2AsRegion<GeomType> ballAsRegion(ball, "MeshGeneration2Test_Sphere2_intersect_5");
   MeshingSettingsStandard<2> settings(5, 1.25);
   Mesh2 mesh;
   mesh.setBaseTriangles(MeshGeneration2::GenerateBaseTriangulation(ballAsRegion, settings));

   const auto bbInitial = settings.getInitialBb(ballAsRegion);
   MeshGeneration2::BaseTriangulationToWorld(mesh.getBaseTriangles(), settings.getGeometryPredicate(), bbInitial, mesh.createPoints(), mesh.getTriangles(), settings.getLogger());

   const auto manifoldPtr = dynamic_cast<const Geometry::IManifold1D2<GeomType>*>(Utilities::Single(ballAsRegion.getManifolds().getAllManifolds()));

   MeshGeneration2::AddManifold1Intersections(*manifoldPtr, mesh.getTriangles(), mesh.setManifoldsAndNodes(), mesh.getSetPoints(), settings.getLogger());

   auto stats = MeshStatistics::Create2(mesh.getTriangles(), mesh.getPoints(), settings.getCellQuality());
   MeshStatistics expect{ 885, 1712, 0.30491369274933333 };
   ASSERT_EQ(expect, stats);

   const auto vtkDataMesh = MeshGeneration2::trianglesToVtkData(mesh, { project, "mesh full" });
   ASSERT_EQ(1712, vtkDataMesh->getNumCells());
   const auto nodesOnManifold = mesh.getManifoldsAndNodes().getNodesInManifold(manifoldPtr);
   mesh.addReconstructions(MeshGeneration2::createReconstructions(ballAsRegion.getManifolds(), mesh.getTriangles(), mesh.getManifoldsAndNodes(), mesh.getPoints()));

   ASSERT_EQ(mesh.getReconstructions().size(), 1);
   const auto reconstruction = dynamic_cast<const Manifold1Reconstruction&>(*mesh.getReconstructions().front()).getReconstruction();

   ASSERT_TRUE(reconstruction.getSingletons().empty());
   ASSERT_TRUE(reconstruction.getPaths().empty());
   ASSERT_EQ(reconstruction.getCycles().size(), 1);
   ASSERT_EQ(Single(reconstruction.getCycles()).size(), 88);
   const auto vtkDataManifold = MeshGeneration2::ToVtkData(reconstruction, mesh.getPoints(), { project, manifoldPtr->getName() });

   Paraview::Write(*vtkDataMesh);
   const std::vector<std::unique_ptr<Vtk::VtkData>> list = reconstructionsToVtkData(mesh, project);
   Paraview::WriteList(list);

   nibble(ballAsRegion, mesh.getReconstructions(), mesh.getTriangles(), mesh.getPoints(), settings.getLogger());

   stats = MeshStatistics::Create2(mesh.getTriangles(), mesh.getPoints(), settings.getCellQuality());
   expect = { 565, 1040, 0.36698742876122409 };
   ASSERT_EQ(expect, stats);
   checkMesh(ballAsRegion, mesh.getTriangles(), mesh.getReconstructions(), mesh.getPoints(), settings);

   const auto vtkDataNibbled = MeshGeneration2::trianglesToVtkData(mesh, { project, "mesh" });
   Paraview::Write(*vtkDataNibbled);
}

TEST(MeshGeneration2Test, Triangle2_1)
{
   const std::string project = "MeshGeneration2Test_Triangle2_1";
   const auto polygonPoints = generateRegularPolygon(3, 0.1); // TODO 0
   const Polygon2AsRegion<double> region(polygonPoints, "triangle");

   MeshingSettingsStandard<2> settings(6, 1.25);
   Mesh2 mesh;
   mesh.setBaseTriangles(MeshGeneration2::GenerateBaseTriangulation(region, settings));

   const auto bbInitial = settings.getInitialBb(region);
   MeshGeneration2::BaseTriangulationToWorld(mesh.getBaseTriangles(), settings.getGeometryPredicate(), bbInitial, mesh.createPoints(), mesh.getTriangles(), settings.getLogger());

   const auto vtkData = MeshGeneration2::trianglesToVtkData(mesh, { project, "BaseTriangulation" });
   Paraview::Write(*vtkData);

   std::vector<const IManifold0D2*> manifolds0 = region.getManifoldsOfType<const IManifold0D2*>();
   MeshGeneration2::AddAllManifolds0(manifolds0, mesh.getTriangles(), mesh.setManifoldsAndNodes(), mesh.getSetPoints(), settings.getLogger());

   for (const auto* manifold1 : region.getManifoldsOfType<const IManifold1D2<GeomType>*>())
   {
      MeshGeneration2::AddManifold1Intersections(*manifold1, mesh.getTriangles(), mesh.setManifoldsAndNodes(), mesh.getSetPoints(), settings.getLogger());
   }

   mesh.addReconstructions(MeshGeneration2::createReconstructions(region, mesh.getTriangles(), mesh.getManifoldsAndNodes(), mesh.getPoints()));

   const auto vtkDataMesh = MeshGeneration2::trianglesToVtkData(mesh, { project, "mesh before nibbling" });
   Paraview::Write(*vtkDataMesh);

   const std::vector<std::unique_ptr<Vtk::VtkData>> list = reconstructionsToVtkData(mesh, project);
   Paraview::WriteList(list);

   nibble(region, mesh.getReconstructions(), mesh.getTriangles(), mesh.getPoints(), settings.getLogger());
   ASSERT_EQ(mesh.getTriangles().getAllTriangles().size(), 2297);
   checkMesh(region, mesh.getTriangles(), mesh.getReconstructions(), mesh.getPoints(), settings);

   const auto stats = MeshStatistics::Create2(mesh.getTriangles(), mesh.getPoints(), settings.getCellQuality());
   const MeshStatistics expect = { 1233, 2297, 0.21379225557104328 };
   ASSERT_EQ(expect, stats);

   const auto vtkDataNibbled = MeshGeneration2::trianglesToVtkData(mesh, { project, "mesh" });
   Paraview::Write(*vtkDataNibbled);
}

TEST(MeshGeneration2Test, Triangle2D_2)
{
   const std::string project = "MeshGeneration2Test_Triangle2D_2";
   const auto polygonPoints = generateRegularPolygon(3, 0.0);
   const Polygon2AsRegion<double> region(polygonPoints, "triangle");

   MeshingSettingsStandard<2> settings(4, 1.25);
   Mesh2 mesh;
   mesh.setBaseTriangles(MeshGeneration2::GenerateBaseTriangulation(region, settings));

   const auto bbInitial = settings.getInitialBb(region);
   MeshGeneration2::BaseTriangulationToWorld(mesh.getBaseTriangles(), settings.getGeometryPredicate(), bbInitial, mesh.createPoints(), mesh.getTriangles(), settings.getLogger());

   std::vector<const IManifold0D2*> manifolds0 = region.getManifoldsOfType<const IManifold0D2*>();
   MeshGeneration2::AddAllManifolds0(manifolds0, mesh.getTriangles(), mesh.setManifoldsAndNodes(), mesh.getSetPoints(), settings.getLogger());

   for (const auto* manifold1 : region.getManifoldsOfType<const IManifold1D2<GeomType>*>())
   {
      MeshGeneration2::AddManifold1Intersections(*manifold1, mesh.getTriangles(), mesh.setManifoldsAndNodes(), mesh.getSetPoints(), settings.getLogger());
   }

   mesh.addReconstructions(MeshGeneration2::createReconstructions(region, mesh.getTriangles(), mesh.getManifoldsAndNodes(), mesh.getPoints()));

   const auto vtkDataMesh = MeshGeneration2::trianglesToVtkData(mesh, { project, "mesh full" });
   Paraview::Write(*vtkDataMesh);

   const std::vector<std::unique_ptr<Vtk::VtkData>> list = reconstructionsToVtkData(mesh, project);
   Paraview::WriteList(list);

   nibble(region, mesh.getReconstructions(), mesh.getTriangles(), mesh.getPoints(), settings.getLogger());

   const auto stats = MeshStatistics::Create2(mesh.getTriangles(), mesh.getPoints(), settings.getCellQuality());
   const MeshStatistics expect = { 99, 154, 0.19197980850908997 };
   ASSERT_EQ(expect, stats);

   const auto vtkDataNibbled = MeshGeneration2::trianglesToVtkData(mesh, { project, "mesh" });
   Paraview::Write(*vtkDataNibbled);

   EdgeFlip edgeFlip(mesh.getTriangles(), settings.getCellQuality(), mesh.getPoints(), mesh.getReconstructions());

   const EdgeFlipStrategy strategy{ 0.25, 2 };
   edgeFlip.execute(strategy);

   checkMesh(region, mesh.getTriangles(), mesh.getReconstructions(), mesh.getPoints(), settings);
}