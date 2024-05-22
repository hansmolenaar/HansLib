#include <gtest/gtest.h>

#include "IndexTreeScaled.h"
#include "RefinementPredicates.h"
#include "Ball.h"
#include "IntervalTreeIndexFactory.h"
#include "MeshGeneration2.h"
#include "InitialBoundingboxGenerator.h"
#include "Ball2AsRegion.h"
#include "Paraview.h"
#include "PointClose.h"
#include "Ball2AsRegion.h"
#include "UniquePointCollectionBinning.h"
#include "Manifold0.h"
#include "Single.h"
#include "Manifold1Reconstruction.h"
#include "MeshingSettingsStandard.h"

using namespace MeshGeneration;
using namespace MeshGeneration2;
using namespace IntervalTree;
using namespace Geometry;
using namespace Utilities;

TEST(MeshGeneration2Test, Ball)
{
   const Ball<GeomType, GeomDim2> ball(Point2{ 0.5, 0.5 }, 0.5);
   const Ball2AsRegion<GeomType> ballAsRegion(ball);
   MeshingSettingsStandard<2> settings(5, 2.0);
   const auto triangles = MeshGeneration2::GenerateBaseTriangulation(ballAsRegion, settings);
   const auto vtkData = IndexTreeToSimplices2::ToVtkData(triangles);
   ASSERT_EQ(1016, vtkData->getNumCells());
   //Paraview::Write("MeshGeneration2Test_Ball", *vtkData);
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

   ASSERT_EQ(trianglesNodes->getAllTriangles().size(), 1);

   //const auto vtkData = MeshGeneration2::ToVtkData(*triangleNodes, *pointGeometry);
   //Paraview::Write("MeshGeneration2Test_SingleTriangleToWorld", *vtkData);
}


TEST(MeshGeneration2Test, Ball2)
{
   const Ball<GeomType, GeomDim2> ball(Point2{ 1.5, 2.5 }, 3);
   const Ball2AsRegion<GeomType> ballAsRegion(ball);
   MeshingSettingsStandard<2> settings(4, 1.25);
   const auto bbInitial = settings.getInitialBbGenerator().generate(ballAsRegion);
   const auto triangles = MeshGeneration2::GenerateBaseTriangulation(ballAsRegion, settings);

   std::unique_ptr<IDynamicUniquePointCollection<GeomType, GeomDim2>> pointGeometry;
   std::unique_ptr<MeshGeneration::TrianglesNodes> trianglesNodes;
   MeshGeneration2::BaseTriangulationToWorld(triangles, settings.getGeometryPredicate(), bbInitial, pointGeometry, trianglesNodes, settings.getLogger());

   const auto vtkData = MeshGeneration2::ToVtkData(*trianglesNodes, *pointGeometry);
   ASSERT_EQ(504, vtkData->getNumCells());
   Paraview::Write("MeshGeneration2Test_Ball2", *vtkData);
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
   const DirectedEdgeNodes edge{ node0, node1 };

   const bool anyNodeMoved = AddEdgeManifold1Intersections(manifold, edge, trianglesNodes, manifoldsAndNodes, points);
   ASSERT_TRUE(anyNodeMoved);
   ASSERT_TRUE(predicate.SamePoints(Point2{ 0,-1 }, points.getPoint(node1)));
}


TEST(MeshGeneration2Test, Sphere2AndEdge_immobilePoint)
{
   Logger logger;
   const Sphere<GeomType, GeomDim2> sphere(Point2{ 0, 0 }, 1);
   const auto bb = sphere.getBoundingBox();
   const Manifold0<GeomType, GeomDim2> pointManifold(Point2{ 0,-1 });
   const Sphere2AsManifold1<GeomType> manifold(sphere);
   const PointClose<GeomType, GeomDim2> predicate;
   UniquePointCollectionBinning<GeomDim2> points(predicate, std::vector < Point2>{bb.getLower(), bb.getUpper()});
   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;
   TrianglesNodes trianglesNodes;
   const auto cellId = trianglesNodes.addTriangle(points.addIfNew(Point2{ -1, -2 }), points.addIfNew(Point2{ 1, -2 }), points.addIfNew(Point2{ 0, -0.9 }));

   const NodeIndex node0 = points.addIfNew(Point2{ 0, -2 });
   const NodeIndex node1 = points.addIfNew(Point2{ 0, -0.9 });
   const NodeIndex node2 = points.addIfNew(Point2{ 0, -1 });
   const DirectedEdgeNodes edge{ node0, node1 };
   manifoldsAndNodes.addNodeToManifold(node2, &pointManifold);

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
   const DirectedEdgeNodes edge{ node0, node1 };

   const bool anyNodeMoved = AddEdgeManifold1Intersections(manifold, edge, trianglesNodes, manifoldsAndNodes, points);
   ASSERT_FALSE(anyNodeMoved);
   ASSERT_TRUE(manifoldsAndNodes.getNodesInManifold(&manifold).empty());
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
   const DirectedEdgeNodes edge{ node0, node1 };

   const bool anyNodeMoved = AddEdgeManifold1Intersections(manifold, edge, trianglesNodes, manifoldsAndNodes, points);
   ASSERT_FALSE(anyNodeMoved);
   ASSERT_EQ(Utilities::Single(manifoldsAndNodes.getNodesInManifold(&manifold)), node0);
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
   const DirectedEdgeNodes edge{ node0, node1 };

   const bool anyNodeMoved = AddEdgeManifold1Intersections(manifold, edge, trianglesNodes, manifoldsAndNodes, points);
   ASSERT_FALSE(anyNodeMoved);
   ASSERT_EQ(Utilities::Single(manifoldsAndNodes.getNodesInManifold(&manifold)), node1);
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
   const DirectedEdgeNodes edge{ node0, node1 };

   const bool anyNodeMoved = AddEdgeManifold1Intersections(manifold, edge, trianglesNodes, manifoldsAndNodes, points);
   ASSERT_FALSE(anyNodeMoved);
   ASSERT_EQ(manifoldsAndNodes.getNodesInManifold(&manifold).size(), 2);
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
   const DirectedEdgeNodes edge{ node0, node1 };

   const bool anyNodeMoved = AddEdgeManifold1Intersections(manifold, edge, trianglesNodes, manifoldsAndNodes, points);
   ASSERT_TRUE(anyNodeMoved);
   ASSERT_EQ(manifoldsAndNodes.getNodesInManifold(&manifold).size(), 2);
   ASSERT_TRUE(predicate.SamePoints(points.getPoint(node1), Point2{ 0, -1 }));
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
   const DirectedEdgeNodes edge{ node0, node1 };

   const bool anyNodeMoved = AddEdgeManifold1Intersections(manifold, edge, trianglesNodes, manifoldsAndNodes, points);
   ASSERT_TRUE(anyNodeMoved);
   ASSERT_EQ(manifoldsAndNodes.getNodesInManifold(&manifold).size(), 2);
   ASSERT_TRUE(predicate.SamePoints(points.getPoint(node0), Point2{ 0, -1 }));
   ASSERT_TRUE(predicate.SamePoints(points.getPoint(node1), Point2{ 0, 1 }));
}

TEST(MeshGeneration2Test, Sphere2_intersect_4)
{
   const Ball<GeomType, GeomDim2> ball(Point2{ 1.5, 2.5 }, 3);
   const Ball2AsRegion<GeomType> ballAsRegion(ball);
   MeshingSettingsStandard<2> settings(4, 1.25);
   const auto triangles = MeshGeneration2::GenerateBaseTriangulation(ballAsRegion, settings);

   std::unique_ptr<IDynamicUniquePointCollection<GeomType, GeomDim2>> pointGeometry;
   std::unique_ptr<MeshGeneration::TrianglesNodes> trianglesNodes;
   const auto bbInitial = settings.getInitialBbGenerator().generate(ballAsRegion);
   MeshGeneration2::BaseTriangulationToWorld(triangles, settings.getGeometryPredicate(), bbInitial, pointGeometry, trianglesNodes, settings.getLogger());

   const Sphere<GeomType, GeomDim2> sphere(ball.getCenter(), ball.getRadius());
   const Sphere2AsManifold1<GeomType> manifold(sphere);

   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;
   MeshGeneration2::AddManifold1Intersections(manifold, *trianglesNodes, manifoldsAndNodes, *pointGeometry);

   const auto vtkData = MeshGeneration2::ToVtkData(*trianglesNodes, *pointGeometry);
   ASSERT_EQ(504, vtkData->getNumCells());
   const auto nodesOnManifold = manifoldsAndNodes.getNodesInManifold(&manifold);
   const auto reconstruction = Manifold1Reconstruction::Generate2(nodesOnManifold, *trianglesNodes, *pointGeometry);
   ASSERT_TRUE(reconstruction.Singletons.empty());
   ASSERT_TRUE(reconstruction.Paths.empty());
   ASSERT_EQ(reconstruction.Cycles.size(), 1);
   ASSERT_EQ(Single(reconstruction.Cycles).size(), 42);
   //Paraview::Write("MeshGeneration2Test_Sphere2_intersect_4", *vtkData);
}

TEST(MeshGeneration2Test, Sphere2_intersect_3)
{
   const Ball<GeomType, GeomDim2> ball(Point2{ 1.5, 2.5 }, 3);
   const Ball2AsRegion<GeomType> ballAsRegion(ball);
   MeshingSettingsStandard<2> settings(3, 1.25);
   const auto triangles = MeshGeneration2::GenerateBaseTriangulation(ballAsRegion, settings);

   std::unique_ptr<IDynamicUniquePointCollection<GeomType, GeomDim2>> pointGeometry;
   std::unique_ptr<MeshGeneration::TrianglesNodes> trianglesNodes;
   const auto bbInitial = settings.getInitialBbGenerator().generate(ballAsRegion);
   MeshGeneration2::BaseTriangulationToWorld(triangles, settings.getGeometryPredicate(), bbInitial, pointGeometry, trianglesNodes, settings.getLogger());

   const Sphere<GeomType, GeomDim2> sphere(ball.getCenter(), ball.getRadius());
   const Sphere2AsManifold1<GeomType> manifold(sphere);

   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;
   MeshGeneration2::AddManifold1Intersections(manifold, *trianglesNodes, manifoldsAndNodes, *pointGeometry);

   const auto vtkData = MeshGeneration2::ToVtkData(*trianglesNodes, *pointGeometry);
   ASSERT_EQ(128, vtkData->getNumCells());
   const auto nodesOnManifold = manifoldsAndNodes.getNodesInManifold(&manifold);
   const auto reconstruction = Manifold1Reconstruction::Generate2(nodesOnManifold, *trianglesNodes, *pointGeometry);
   ASSERT_TRUE(reconstruction.Singletons.empty());
   ASSERT_TRUE(reconstruction.Paths.empty());
   ASSERT_EQ(reconstruction.Cycles.size(), 1);
   ASSERT_EQ(Single(reconstruction.Cycles).size(), 20);
   //Paraview::Write("MeshGeneration2Test_Sphere2_intersect_3", *vtkData);
}

TEST(MeshGeneration2Test, Sphere2_intersect_5)
{
   const Ball<GeomType, GeomDim2> ball(Point2{ 1.5, 2.5 }, 3);
   const Ball2AsRegion<GeomType> ballAsRegion(ball);
   MeshingSettingsStandard<2> settings( 5, 1.25);
   const auto triangles = MeshGeneration2::GenerateBaseTriangulation(ballAsRegion, settings);

   std::unique_ptr<IDynamicUniquePointCollection<GeomType, GeomDim2>> pointGeometry;
   std::unique_ptr<MeshGeneration::TrianglesNodes> trianglesNodes;
   const auto bbInitial = settings.getInitialBbGenerator().generate(ballAsRegion);
   MeshGeneration2::BaseTriangulationToWorld(triangles, settings.getGeometryPredicate(), bbInitial, pointGeometry, trianglesNodes, settings.getLogger());

   const Sphere<GeomType, GeomDim2> sphere(ball.getCenter(), ball.getRadius());
   const Sphere2AsManifold1<GeomType> manifold(sphere);

   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;
   MeshGeneration2::AddManifold1Intersections(manifold, *trianglesNodes, manifoldsAndNodes, *pointGeometry);

   const auto vtkData = MeshGeneration2::ToVtkData(*trianglesNodes, *pointGeometry);
   ASSERT_EQ(1712, vtkData->getNumCells());
   const auto nodesOnManifold = manifoldsAndNodes.getNodesInManifold(&manifold);
   const auto reconstruction = Manifold1Reconstruction::Generate2(nodesOnManifold, *trianglesNodes, *pointGeometry);
   ASSERT_TRUE(reconstruction.Singletons.empty());
   ASSERT_TRUE(reconstruction.Paths.empty());
   ASSERT_EQ(reconstruction.Cycles.size(), 1);
   ASSERT_EQ(Single(reconstruction.Cycles).size(), 88);
   Paraview::Write("MeshGeneration2Test_Sphere2_intersect_5", *vtkData);
}
