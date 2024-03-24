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

using namespace MeshGeneration;
using namespace IntervalTree;
using namespace Geometry;

TEST(MeshGeneration2Test, Ball)
{
   Logger logger;
   const Ball<double, 2> ball(Point2{ 0.5, 0.5 }, 0.5);
   const Ball2AsRegion<double> ballAsRegion(ball);
   const PointClose<double, 2> areClose;
   const auto initialBbGenerator = InitialBoundingboxGenerator<2>::Create(2.0);
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
   baseTriangles.push_back({rp0, rp1, rp2});
   const auto bb = BoundingBox<double, 2>::CreateFrom2Points(Point2{ 1,1 }, Point2{ 2,3 });
   const PointClose<double, 2> areClose;

   std::unique_ptr<IDynamicUniquePointCollection<double, 2>> pointGeometry;
   std::unique_ptr<MeshGeneration::TriangleNodes> triangleNodes;

   MeshGeneration2::BaseTriangulationToWorld(baseTriangles, areClose, bb, pointGeometry, triangleNodes, logger);

   ASSERT_EQ(pointGeometry->getNumPoints(), 3);
   ASSERT_TRUE(areClose(pointGeometry->getPoint(0), Point2{1,1}));
   ASSERT_TRUE(areClose(pointGeometry->getPoint(1), Point2{ 2,1 }));
   ASSERT_TRUE(areClose(pointGeometry->getPoint(2), Point2{ 1,3 }));

   ASSERT_EQ(triangleNodes->getAllTriangles().size(), 1);

   //const auto vtkData = MeshGeneration2::ToVtkData(*triangleNodes, *pointGeometry);
   //Paraview::Write("MeshGeneration2Test_SingleTriangleToWorld", *vtkData);
}


TEST(MeshGeneration2Test, Ball2)
{
   Logger logger;
   const Ball<double, 2> ball(Point2{ 1.5, 2.5 }, 3);
   const Ball2AsRegion<double> ballAsRegion(ball);
   const PointClose<double,2> areClose;
   const auto initialBbGenerator = InitialBoundingboxGenerator<2>::Create(1.25);
   const auto bbInitial = initialBbGenerator->generate(ballAsRegion);
   const RefineRegionToMaxLevel<2> predicate(4, ballAsRegion, areClose, *initialBbGenerator);
   MeshingStrategy2 strategy(*initialBbGenerator, predicate);
   const auto triangles = MeshGeneration2::GenerateBaseTriangulation(ballAsRegion, strategy, logger);

   std::unique_ptr<IDynamicUniquePointCollection<double, 2>> pointGeometry;
   std::unique_ptr<MeshGeneration::TriangleNodes> triangleNodes;
   MeshGeneration2::BaseTriangulationToWorld(triangles, areClose, bbInitial, pointGeometry, triangleNodes, logger);

   const auto vtkData = MeshGeneration2::ToVtkData(*triangleNodes, *pointGeometry);
   ASSERT_EQ(504, vtkData->getNumCells());
   //Paraview::Write("MeshGeneration2Test_Ball2", *vtkData);
}

