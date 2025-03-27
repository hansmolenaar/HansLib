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

using namespace MeshGeneration;
using namespace MeshGeneration2;
using namespace IntervalTree;
using namespace Geometry;
using namespace Utilities;

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
}

TEST(MeshGeneration2Test, Ball)
{
   const Ball<GeomType, GeomDim2> ball(Point2{ 0.5, 0.5 }, 0.5);
   const Ball2AsRegion<GeomType> ballAsRegion(ball, "MeshGeneration2Test_Bal");
   MeshingSettingsStandard<2> settings(5, 2.0);
   IRegionManifoldsTestInterface(ballAsRegion.getManifolds(), settings.getGeometryPredicate());
   const auto triangles = MeshGeneration2::GenerateBaseTriangulation(ballAsRegion, settings);
   const auto vtkData = IndexTreeToSimplices2::ToVtkData(triangles, { "MeshGeneration2Test_Ball" , "triangles" });
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

   std::unique_ptr<IDynamicUniquePointCollection<GeomType, GeomDim2>> pointGeometry;
   std::unique_ptr<MeshGeneration::TrianglesNodes> trianglesNodes;

   MeshGeneration2::BaseTriangulationToWorld(baseTriangles, areClose, bb, pointGeometry, trianglesNodes, logger);

   ASSERT_EQ(pointGeometry->getNumPoints(), 3);
   ASSERT_TRUE(areClose(pointGeometry->getPoint(0), Point2{ 1,1 }));
   ASSERT_TRUE(areClose(pointGeometry->getPoint(1), Point2{ 2,1 }));
   ASSERT_TRUE(areClose(pointGeometry->getPoint(2), Point2{ 1,3 }));
   checkTriangleArea(*trianglesNodes, *pointGeometry);

   ASSERT_EQ(trianglesNodes->getAllTriangles().size(), 1);

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
   const auto triangles = MeshGeneration2::GenerateBaseTriangulation(ballAsRegion, settings);

   std::unique_ptr<IDynamicUniquePointCollection<GeomType, GeomDim2>> pointGeometry;
   std::unique_ptr<MeshGeneration::TrianglesNodes> trianglesNodes;
   MeshGeneration2::BaseTriangulationToWorld(triangles, settings.getGeometryPredicate(), bbInitial, pointGeometry, trianglesNodes, settings.getLogger());

   const auto stats = MeshStatistics::Create2(*trianglesNodes, *pointGeometry, settings.getCellQuality());
   checkTriangleArea(*trianglesNodes, *pointGeometry);
   const MeshStatistics expect{ 281, 504, 0.75 };
   ASSERT_EQ(stats, expect);

   const auto vtkData = MeshGeneration2::ToVtkData(*trianglesNodes, *pointGeometry, { project, "mesh" });
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
   const auto cellId = trianglesNodes.addTriangle(points.addIfNew(Point2{ -1, -2 }), points.addIfNew(Point2{ 1, -2 }), points.addIfNew(Point2{ 0, -0.9 }));

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
   const auto cellId = trianglesNodes.addTriangle(points.addIfNew(Point2{ -1, -2 }), points.addIfNew(Point2{ 1, -2 }), points.addIfNew(Point2{ 0, -0.9 }));

   const NodeIndex node0 = points.addIfNew(Point2{ 0, -2 });
   const NodeIndex node1 = points.addIfNew(Point2{ 0, -0.9 });
   const NodeIndex node2 = points.addIfNew(Point2{ 0, -1 });
   const EdgeNodesDirected edge{ node0, node1 };
   manifoldsAndNodes.addNodeToManifold(node2, &pointManifold);
   checkTriangleArea(trianglesNodes, points);

   ASSERT_THROW(AddEdgeManifold1Intersections(manifold, edge, trianglesNodes, manifoldsAndNodes, points), MyException);
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
   IRegionManifoldsTestInterface(ballAsRegion.getManifolds(), settings.getGeometryPredicate());
   const auto triangles = MeshGeneration2::GenerateBaseTriangulation(ballAsRegion, settings);

   std::unique_ptr<IDynamicUniquePointCollection<GeomType, GeomDim2>> pointGeometry;
   std::unique_ptr<MeshGeneration::TrianglesNodes> trianglesNodes;
   const auto bbInitial = settings.getInitialBb(ballAsRegion);
   MeshGeneration2::BaseTriangulationToWorld(triangles, settings.getGeometryPredicate(), bbInitial, pointGeometry, trianglesNodes, settings.getLogger());

   auto stats = MeshStatistics::Create2(*trianglesNodes, *pointGeometry, settings.getCellQuality());
   MeshStatistics expect{ 281, 504, 0.75 };
   ASSERT_EQ(expect, stats);

   const auto manifoldPtr = dynamic_cast<const Geometry::IManifold1D2<GeomType>*>(Utilities::Single(ballAsRegion.getManifolds().getAllManifolds()));

   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;
   MeshGeneration2::AddManifold1Intersections(*manifoldPtr, *trianglesNodes, manifoldsAndNodes, *pointGeometry, settings.getLogger());

   stats = MeshStatistics::Create2(*trianglesNodes, *pointGeometry, settings.getCellQuality());
   expect = { 281, 504, 0.37443649960593806 };

   settings.getLogger().toFile("C:\\Users\\Hans\\Documents\\work\\logger.txt");
   ASSERT_EQ(expect, stats);

   const auto nodesOnManifold = manifoldsAndNodes.getNodesInManifold(manifoldPtr);
   const auto reconstructions = MeshGeneration2::createReconstructions(ballAsRegion.getManifolds(), *trianglesNodes, manifoldsAndNodes, *pointGeometry);
   const bool succes = MeshGeneration2::checkReconstructions(ballAsRegion.getManifolds(), reconstructions, settings.getLogger());
   ASSERT_TRUE(succes);
   ASSERT_EQ(reconstructions.size(), 1);
   const auto reconstruction = dynamic_cast<const Manifold1Reconstruction&>(*reconstructions.front()).getReconstruction();
   ASSERT_TRUE(reconstruction.Singletons.empty());
   ASSERT_TRUE(reconstruction.Paths.empty());
   ASSERT_EQ(reconstruction.Cycles.size(), 1);
   ASSERT_EQ(Single(reconstruction.Cycles).size(), 42);

   const auto vtkData = MeshGeneration2::ToVtkData(*trianglesNodes, *pointGeometry, { project, "mesh full" });
   const auto vtkDataManifold = MeshGeneration2::ToVtkData(reconstruction, *pointGeometry, { project, manifoldPtr->getName() });

   Paraview::Write(*vtkData);
   Paraview::Write(*(Utilities::Single(vtkDataManifold)));

   nibble(ballAsRegion, reconstructions, *trianglesNodes, *pointGeometry, settings.getLogger());
   ASSERT_EQ(trianglesNodes->getAllTriangles().size(), 250);
   ASSERT_EQ(trianglesNodes->getAllNodes().size(), 147);
   checkTriangleArea(*trianglesNodes, *pointGeometry);

   stats = MeshStatistics::Create2(*trianglesNodes, *pointGeometry, settings.getCellQuality());
   expect = { 147, 250, 0.37443649960593806 };
   ASSERT_EQ(expect, stats);

   const auto vtkDataNibbled = MeshGeneration2::ToVtkData(*trianglesNodes, *pointGeometry, { project, "mesh" });
   Paraview::Write(*vtkDataNibbled);
}

TEST(MeshGeneration2Test, Sphere2_intersect_3)
{
   const std::string project = "MeshGeneration2Test_Sphere2_intersect_3";
   const Ball<GeomType, GeomDim2> ball(Point2{ 1.5, 2.5 }, 3);
   const Ball2AsRegion<GeomType> ballAsRegion(ball, "MeshGeneration2Test_Sphere2_intersect_3");
   MeshingSettingsStandard<2> settings(3, 1.25);
   IRegionManifoldsTestInterface(ballAsRegion.getManifolds(), settings.getGeometryPredicate());
   const auto triangles = MeshGeneration2::GenerateBaseTriangulation(ballAsRegion, settings);

   std::unique_ptr<IDynamicUniquePointCollection<GeomType, GeomDim2>> pointGeometry;
   std::unique_ptr<MeshGeneration::TrianglesNodes> trianglesNodes;
   const auto bbInitial = settings.getInitialBb(ballAsRegion);
   MeshGeneration2::BaseTriangulationToWorld(triangles, settings.getGeometryPredicate(), bbInitial, pointGeometry, trianglesNodes, settings.getLogger());

   const auto manifoldPtr = dynamic_cast<const Geometry::IManifold1D2<GeomType>*>(Utilities::Single(ballAsRegion.getManifolds().getAllManifolds()));

   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;
   MeshGeneration2::AddManifold1Intersections(*manifoldPtr, *trianglesNodes, manifoldsAndNodes, *pointGeometry, settings.getLogger());

   auto stats = MeshStatistics::Create2(*trianglesNodes, *pointGeometry, settings.getCellQuality());
   MeshStatistics expect{ 81, 128, 00.30877886910687341 };
   ASSERT_EQ(expect, stats);

   const auto vtkData = MeshGeneration2::ToVtkData(*trianglesNodes, *pointGeometry, { project, "mesh full" });
   ASSERT_EQ(128, vtkData->getNumCells());
   const auto nodesOnManifold = manifoldsAndNodes.getNodesInManifold(manifoldPtr);
   const auto reconstructions = MeshGeneration2::createReconstructions(ballAsRegion.getManifolds(), *trianglesNodes, manifoldsAndNodes, *pointGeometry);
   const bool succes = MeshGeneration2::checkReconstructions(ballAsRegion.getManifolds(), reconstructions, settings.getLogger());
   ASSERT_TRUE(succes);
   ASSERT_EQ(reconstructions.size(), 1);
   const auto reconstruction = dynamic_cast<const Manifold1Reconstruction&>(*reconstructions.front()).getReconstruction();

   ASSERT_TRUE(reconstruction.Singletons.empty());
   ASSERT_TRUE(reconstruction.Paths.empty());
   ASSERT_EQ(reconstruction.Cycles.size(), 1);
   ASSERT_EQ(Single(reconstruction.Cycles).size(), 20);
   const auto vtkDataManifold = MeshGeneration2::ToVtkData(reconstruction, *pointGeometry, { project, manifoldPtr->getName() });

   Paraview::Write(*vtkData);
   Paraview::Write(*(Utilities::Single(vtkDataManifold)));

   ASSERT_EQ(trianglesNodes->getAllTriangles().size(), 128);
   ASSERT_EQ(trianglesNodes->getAllNodes().size(), 81);
   nibble(ballAsRegion, reconstructions, *trianglesNodes, *pointGeometry, settings.getLogger());
   ASSERT_EQ(trianglesNodes->getAllTriangles().size(), 60);
   ASSERT_EQ(trianglesNodes->getAllNodes().size(), 41);
   checkTriangleArea(*trianglesNodes, *pointGeometry);


   stats = MeshStatistics::Create2(*trianglesNodes, *pointGeometry, settings.getCellQuality());
   expect = { 41, 60, 0.55410066890581655 };
   ASSERT_EQ(expect, stats);

   const auto vtkDataNibbled = MeshGeneration2::ToVtkData(*trianglesNodes, *pointGeometry, { project, "mesh" });
   Paraview::Write(*vtkDataNibbled);
}

TEST(MeshGeneration2Test, Sphere2_intersect_5)
{
   const std::string project = "MeshGeneration2Test_Sphere2_intersect_5";
   const Ball<GeomType, GeomDim2> ball(Point2{ 1.5, 2.5 }, 3);
   const Ball2AsRegion<GeomType> ballAsRegion(ball, "MeshGeneration2Test_Sphere2_intersect_5");
   MeshingSettingsStandard<2> settings(5, 1.25);
   IRegionManifoldsTestInterface(ballAsRegion.getManifolds(), settings.getGeometryPredicate());
   const auto triangles = MeshGeneration2::GenerateBaseTriangulation(ballAsRegion, settings);

   std::unique_ptr<IDynamicUniquePointCollection<GeomType, GeomDim2>> pointGeometry;
   std::unique_ptr<MeshGeneration::TrianglesNodes> trianglesNodes;
   const auto bbInitial = settings.getInitialBb(ballAsRegion);
   MeshGeneration2::BaseTriangulationToWorld(triangles, settings.getGeometryPredicate(), bbInitial, pointGeometry, trianglesNodes, settings.getLogger());

   const auto manifoldPtr = dynamic_cast<const Geometry::IManifold1D2<GeomType>*>(Utilities::Single(ballAsRegion.getManifolds().getAllManifolds()));

   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;
   MeshGeneration2::AddManifold1Intersections(*manifoldPtr, *trianglesNodes, manifoldsAndNodes, *pointGeometry, settings.getLogger());

   auto stats = MeshStatistics::Create2(*trianglesNodes, *pointGeometry, settings.getCellQuality());
   MeshStatistics expect{ 885, 1712, 0.30491369274933333 };
   ASSERT_EQ(expect, stats);

   const auto vtkDataMesh = MeshGeneration2::ToVtkData(*trianglesNodes, *pointGeometry, { project, "mesh full" });
   ASSERT_EQ(1712, vtkDataMesh->getNumCells());
   const auto nodesOnManifold = manifoldsAndNodes.getNodesInManifold(manifoldPtr);
   const auto reconstructions = MeshGeneration2::createReconstructions(ballAsRegion.getManifolds(), *trianglesNodes, manifoldsAndNodes, *pointGeometry);
   const bool succes = MeshGeneration2::checkReconstructions(ballAsRegion.getManifolds(), reconstructions, settings.getLogger());
   ASSERT_TRUE(succes);
   ASSERT_EQ(reconstructions.size(), 1);
   const auto reconstruction = dynamic_cast<const Manifold1Reconstruction&>(*reconstructions.front()).getReconstruction();

   ASSERT_TRUE(reconstruction.Singletons.empty());
   ASSERT_TRUE(reconstruction.Paths.empty());
   ASSERT_EQ(reconstruction.Cycles.size(), 1);
   ASSERT_EQ(Single(reconstruction.Cycles).size(), 88);
   const auto vtkDataManifold = MeshGeneration2::ToVtkData(reconstruction, *pointGeometry, { project, manifoldPtr->getName() });

   Paraview::Write(*vtkDataMesh);
   const std::vector<std::unique_ptr<Vtk::VtkData>> list = ToVtkData(reconstructions, *pointGeometry, project);
   Paraview::WriteList(list);

   nibble(ballAsRegion, reconstructions, *trianglesNodes, *pointGeometry, settings.getLogger());

   stats = MeshStatistics::Create2(*trianglesNodes, *pointGeometry, settings.getCellQuality());
   expect = { 565, 1040, 0.36698742876122409 };
   ASSERT_EQ(expect, stats);
   checkTriangleArea(*trianglesNodes, *pointGeometry);

   const auto vtkDataNibbled = MeshGeneration2::ToVtkData(*trianglesNodes, *pointGeometry, { project, "mesh" });
   Paraview::Write(*vtkDataNibbled);
}

TEST(MeshGeneration2Test, Triangle2_1)
{
   const std::string project = "MeshGeneration2Test_Triangle2_1";
   const auto polygonPoints = generateRegularPolygon(3, 0.1); // TODO 0
   const Polygon2AsRegion<double> region(polygonPoints, "triangle");

   MeshingSettingsStandard<2> settings(6, 1.25);
   IRegionManifoldsTestInterface(region.getManifolds(), settings.getGeometryPredicate());
   const auto triangles = MeshGeneration2::GenerateBaseTriangulation(region, settings);

   std::unique_ptr<IDynamicUniquePointCollection<GeomType, GeomDim2>> pointGeometry;
   std::unique_ptr<MeshGeneration::TrianglesNodes> trianglesNodes;
   const auto bbInitial = settings.getInitialBb(region);
   MeshGeneration2::BaseTriangulationToWorld(triangles, settings.getGeometryPredicate(), bbInitial, pointGeometry, trianglesNodes, settings.getLogger());

   const auto vtkData = MeshGeneration2::ToVtkData(*trianglesNodes, *pointGeometry, { project, "BaseTriangulation" });
   Paraview::Write(*vtkData);

   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;
   std::vector<const IManifold0D2*> manifolds0 = region.getManifoldsOfType<const IManifold0D2*>();
   MeshGeneration2::AddAllManifolds0(manifolds0, *trianglesNodes, manifoldsAndNodes, *pointGeometry, settings.getLogger());

   for (const auto* manifold1 : region.getManifoldsOfType<const IManifold1D2<GeomType>*>())
   {
      MeshGeneration2::AddManifold1Intersections(*manifold1, *trianglesNodes, manifoldsAndNodes, *pointGeometry, settings.getLogger());
   }

   const auto reconstructions = MeshGeneration2::createReconstructions(region, *trianglesNodes, manifoldsAndNodes, *pointGeometry);
   const bool succes = MeshGeneration2::checkReconstructions(region, reconstructions, settings.getLogger());
   ASSERT_TRUE(succes);

   const auto vtkDataMesh = MeshGeneration2::ToVtkData(*trianglesNodes, *pointGeometry, { project, "mesh before nibbling" });
   Paraview::Write(*vtkDataMesh);

   const std::vector<std::unique_ptr<Vtk::VtkData>> list = ToVtkData(reconstructions, *pointGeometry, project);
   Paraview::WriteList(list);

   nibble(region, reconstructions, *trianglesNodes, *pointGeometry, settings.getLogger());
   ASSERT_EQ(trianglesNodes->getAllTriangles().size(), 2297);
   checkTriangleArea(*trianglesNodes, *pointGeometry);

   const auto stats = MeshStatistics::Create2(*trianglesNodes, *pointGeometry, settings.getCellQuality());
   const MeshStatistics expect = { 1233, 2297, 0.21379225557104328 };
   ASSERT_EQ(expect, stats);

   const auto vtkDataNibbled = MeshGeneration2::ToVtkData(*trianglesNodes, *pointGeometry, { project, "mesh" });
   Paraview::Write(*vtkDataNibbled);
}

TEST(MeshGeneration2Test, Triangle2D_2)
{
   const std::string project = "MeshGeneration2Test_Triangle2D_2";
   const auto polygonPoints = generateRegularPolygon(3, 0.0);
   const Polygon2AsRegion<double> region(polygonPoints, "triangle");

   MeshingSettingsStandard<2> settings(4, 1.25);
   IRegionManifoldsTestInterface(region.getManifolds(), settings.getGeometryPredicate());
   const auto triangles = MeshGeneration2::GenerateBaseTriangulation(region, settings);

   std::unique_ptr<IDynamicUniquePointCollection<GeomType, GeomDim2>> pointGeometry;
   std::unique_ptr<MeshGeneration::TrianglesNodes> trianglesNodes;
   const auto bbInitial = settings.getInitialBb(region);
   MeshGeneration2::BaseTriangulationToWorld(triangles, settings.getGeometryPredicate(), bbInitial, pointGeometry, trianglesNodes, settings.getLogger());

   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;
   std::vector<const IManifold0D2*> manifolds0 = region.getManifoldsOfType<const IManifold0D2*>();
   MeshGeneration2::AddAllManifolds0(manifolds0, *trianglesNodes, manifoldsAndNodes, *pointGeometry, settings.getLogger());

   for (const auto* manifold1 : region.getManifoldsOfType<const IManifold1D2<GeomType>*>())
   {
      MeshGeneration2::AddManifold1Intersections(*manifold1, *trianglesNodes, manifoldsAndNodes, *pointGeometry, settings.getLogger());
   }

   const auto reconstructions = MeshGeneration2::createReconstructions(region, *trianglesNodes, manifoldsAndNodes, *pointGeometry);
   const bool succes = MeshGeneration2::checkReconstructions(region, reconstructions, settings.getLogger());
   ASSERT_TRUE(succes);

   const auto vtkDataMesh = MeshGeneration2::ToVtkData(*trianglesNodes, *pointGeometry, { project, "mesh full" });
   Paraview::Write(*vtkDataMesh);

   const std::vector<std::unique_ptr<Vtk::VtkData>> list = ToVtkData(reconstructions, *pointGeometry, project);
   Paraview::WriteList(list);

   nibble(region, reconstructions, *trianglesNodes, *pointGeometry, settings.getLogger());

   const auto stats = MeshStatistics::Create2(*trianglesNodes, *pointGeometry, settings.getCellQuality());
   const MeshStatistics expect = { 99, 154, 0.19197980850908997 };
   ASSERT_EQ(expect, stats);

   const auto vtkDataNibbled = MeshGeneration2::ToVtkData(*trianglesNodes, *pointGeometry, { project, "mesh" });
   Paraview::Write(*vtkDataNibbled);

   EdgeFlip edgeFlip(*trianglesNodes, settings.getCellQuality(), *pointGeometry, reconstructions);

   const EdgeFlipStrategy strategy{ 0.25, 2 };
   edgeFlip.execute(strategy);

   checkTriangleArea(*trianglesNodes, *pointGeometry);
}