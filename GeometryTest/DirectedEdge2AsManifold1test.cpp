#include <gtest/gtest.h>

#include "DirectedEdge2AsManifold1.h"
#include "IManifold1D2Test.h"
#include "PointClose.h"
#include "Single.h"

using namespace Geometry;
using namespace Topology;

namespace
{
   constexpr double crit = 1.0e-10;
}

TEST(DirectedEdge2AsManifold1, Constructor)
{
   const PointClose<double, GeomDim2> predicate;
   const Point2 p0{ {-1,1} };
   const Point2 p1{ {1,-1} };
   const DirectedEdge2 edge{ p0, p1 };
   const DirectedEdge2AsManifold1<double> edgeManifold(edge);

   IManifold1D2TestInterface(edgeManifold, predicate);

   ASSERT_FALSE(edgeManifold.contains({ 1,1 }, predicate));
   ASSERT_TRUE(edgeManifold.contains({ 0,0 }, predicate));

   const auto bb = edgeManifold.getBoundingBox();
   ASSERT_TRUE(predicate.SamePoints(bb.getLower(), Point2{ -1, -1 }));
   ASSERT_TRUE(predicate.SamePoints(bb.getUpper(), Point2{ 1, 1 }));
}


TEST(DirectedEdge2AsManifold1, GetEuclideanSubspaceAt)
{
   const PointClose<double, GeomDim2> predicate;
   const Point2 p0{ {-1,1} };
   const Point2 p1{ {1,-1} };
   const DirectedEdge2 edge{ p0, p1 };
   const DirectedEdge2AsManifold1<double> edgeManifold(edge);

   const  Line<double, 2> line = edgeManifold.getEuclideanSubspaceAt(Point2{ -0.2, 0.2 }, predicate);
   ASSERT_TRUE(line.contains(Point2{ 0, 0 }, predicate));

   ASSERT_THROW(edgeManifold.getEuclideanSubspaceAt(Point2{ 3,3 }, predicate), MyException);
}

TEST(DirectedEdge2AsManifold1, GetIntersections)
{
   const PointClose<double, GeomDim2> predicate;
   const Point2 p0{ {-1,-1} };
   const Point2 p1{ {1,1} };
   const Point2 p2{ {1,2} };
   const DirectedEdge2 edge{ p0, p1 };
   const DirectedEdge2AsManifold1<double> edgeManifold(edge);

   const auto intersections = edgeManifold.GetIntersections(DirectedEdge2{ p1, p2 }, predicate);
   const auto& intersection = Utilities::Single(intersections.get());
   ASSERT_TRUE(predicate.SamePoints(intersection.getPoint().getPoint(), p1));
}