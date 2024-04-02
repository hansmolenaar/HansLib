#include <gtest/gtest.h>

#include "IndexTreeScaled.h"
#include "RefinementPredicates.h"
#include "Ball.h"
#include "IntervalTreeIndexFactory.h"
#include "MeshGeneration2.h"
#include "MeshingStrategy.h"
#include "InitialBoundingboxGenerator.h"
#include "Ball2AsRegion.h"
#include "Paraview.h"
#include "PointClose.h"
#include "Ball2AsRegion.h"
#include "UniquePointCollectionBinning.h"
#include "Manifold0.h"
#include "Single.h"

using namespace MeshGeneration;
using namespace MeshGeneration2;
using namespace IntervalTree;
using namespace Geometry;

TEST(MeshGeneration2Test, Ball)
{
   Logger logger;
   const Ball<GeomType, GeomDim2> ball(Point2{ 0.5, 0.5 }, 0.5);
   const Ball2AsRegion<GeomType> ballAsRegion(ball);
   const PointClose<GeomType, GeomDim2> areClose;
   const auto initialBbGenerator = InitialBoundingboxGenerator<GeomDim2>::Create(2.0);
   const RefineRegionToMaxLevel<2> predicate(5, ballAsRegion, areClose, *initialBbGenerator);
   MeshingStrategy2 strategy(*initialBbGenerator, predicate);
   const auto triangles = MeshGeneration2::GenerateBaseTriangulation(ballAsRegion, strategy, logger);

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
   Logger logger;
   const Ball<GeomType, GeomDim2> ball(Point2{ 1.5, 2.5 }, 3);
   const Ball2AsRegion<GeomType> ballAsRegion(ball);
   const PointClose<GeomType, GeomDim2> areClose;
   const auto initialBbGenerator = InitialBoundingboxGenerator<GeomDim2>::Create(1.25);
   const auto bbInitial = initialBbGenerator->generate(ballAsRegion);
   const RefineRegionToMaxLevel<GeomDim2> predicate(4, ballAsRegion, areClose, *initialBbGenerator);
   MeshingStrategy2 strategy(*initialBbGenerator, predicate);
   const auto triangles = MeshGeneration2::GenerateBaseTriangulation(ballAsRegion, strategy, logger);

   std::unique_ptr<IDynamicUniquePointCollection<GeomType, GeomDim2>> pointGeometry;
   std::unique_ptr<MeshGeneration::TrianglesNodes> trianglesNodes;
   MeshGeneration2::BaseTriangulationToWorld(triangles, areClose, bbInitial, pointGeometry, trianglesNodes, logger);

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


TEST(MeshGeneration2Test, Sphere2AndEdge_2Intersections)
{
   Logger logger;
   const Sphere<GeomType, GeomDim2> sphere(Point2{ 0, 0 }, 1);
   const auto bb = sphere.getBoundingBox();
   const Sphere2AsManifold1<GeomType> manifold(sphere);
   const PointClose<GeomType, GeomDim2> predicate;
   UniquePointCollectionBinning<GeomDim2> points(predicate, std::vector < Point2>{bb.getLower(), bb.getUpper()});
   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;
   TrianglesNodes trianglesNodes;

   const NodeIndex node0 = points.addIfNew(Point2{ -2, 0 });
   const NodeIndex node1 = points.addIfNew(Point2{ 2, 0 });
   const DirectedEdgeNodes edge{ node0, node1 };

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

TEST(MeshGeneration2Test, Sphere2_intersect)
{
   Logger logger;
   const Ball<GeomType, GeomDim2> ball(Point2{ 1.5, 2.5 }, 3);
   const Ball2AsRegion<GeomType> ballAsRegion(ball);
   const PointClose<GeomType, GeomDim2> areClose;
   const auto initialBbGenerator = InitialBoundingboxGenerator<GeomDim2>::Create(1.25);
   const auto bbInitial = initialBbGenerator->generate(ballAsRegion);
   const RefineRegionToMaxLevel<GeomDim2> predicate(4, ballAsRegion, areClose, *initialBbGenerator);
   MeshingStrategy2 strategy(*initialBbGenerator, predicate);
   const auto triangles = MeshGeneration2::GenerateBaseTriangulation(ballAsRegion, strategy, logger);

   std::unique_ptr<IDynamicUniquePointCollection<GeomType, GeomDim2>> pointGeometry;
   std::unique_ptr<MeshGeneration::TrianglesNodes> trianglesNodes;
   MeshGeneration2::BaseTriangulationToWorld(triangles, areClose, bbInitial, pointGeometry, trianglesNodes, logger);

   const Sphere<GeomType, GeomDim2> sphere(ball.getCenter(), ball.getRadius());
   const Sphere2AsManifold1<GeomType> manifold(sphere);

   ManifoldsAndNodes<GeomDim2> manifoldsAndNodes;
   MeshGeneration2::AddManifold1Intersections(manifold, *trianglesNodes, manifoldsAndNodes, *pointGeometry);

   //const auto vtkData = MeshGeneration2::ToVtkData(*trianglesNodes, *pointGeometry);
   //ASSERT_EQ(504, vtkData->getNumCells());
   //Paraview::Write("MeshGeneration2Test_Ball2", *vtkData);
}