#include <gtest/gtest.h>

#include "IManifold1D2Test.h"
#include "PointClose.h"
#include "Polygon2AsRegion.h"

using namespace Geometry;
using namespace Topology;

namespace
{
   constexpr double crit = 1.0e-10;
}

TEST(Polygon2AsRegion, Base)
{
   const std::string name = "foo";
   const PointClose<double, GeomDim2> predicate;
   const Point2 p{ {2,-3} };
   const std::vector<Point2> trianglePoints{ Point2{0,0}, Point2{1,0}, Point2{0,1} };
   const Polygon2AsRegion<double> triangleRegion(trianglePoints, name);

   ASSERT_FALSE(triangleRegion.contains(p, predicate));
   const std::vector<Point2> pointsInTriangle{ { 0.5, 0.5 }, {0.25, 0.25}, {1,0} };
   for (const auto& p : pointsInTriangle)
   {
      ASSERT_TRUE(triangleRegion.contains(p, predicate));
   }

   ASSERT_EQ(triangleRegion.getName(), "foo");

   const auto bb = triangleRegion.getBoundingBox();
   ASSERT_TRUE(predicate.samePoints(bb.getLower(), { 0,0 }));
   ASSERT_TRUE(predicate.samePoints(bb.getUpper(), { 1,1 }));

   ASSERT_TRUE(triangleRegion.contains({ 0.1, 0.1 }, predicate));
   const auto allManifolds = triangleRegion.getManifolds().getAllManifolds();
   ASSERT_EQ(allManifolds.size(), 6);

   const auto edgeManifolds = triangleRegion.getBoundaryHyperManifolds();
   ASSERT_EQ(edgeManifolds.size(), 3);
   ASSERT_TRUE(str::all_of(edgeManifolds, [](const auto* m) {return m->getTopologyDimension() == Topology::Edge; }));

   std::vector<const IManifold<double, GeomDim2>*> pointManifolds;
   str::copy_if(allManifolds, std::back_inserter(pointManifolds), [](const auto* m) { return m->getTopologyDimension() == Topology::Corner; });
   ASSERT_EQ(pointManifolds.size(), 3);

   ASSERT_TRUE(triangleRegion.getConnectedLowers(*pointManifolds.front()).empty());
   ASSERT_TRUE(triangleRegion.getConnectedHighers(*edgeManifolds.front()).empty());

   const auto edge2_connections = triangleRegion.getConnectedLowers(*edgeManifolds.back());
   ASSERT_EQ(edge2_connections.size(), 2);
   auto point = dynamic_cast<const Manifold0<double, GeomDim2>*>(edge2_connections.front())->getPoint();
   ASSERT_TRUE(predicate.samePoints(point, trianglePoints.at(2)));
   point = dynamic_cast<const Manifold0<double, GeomDim2>*>(edge2_connections.back())->getPoint();
   ASSERT_TRUE(predicate.samePoints(point, trianglePoints.at(0)));

   const auto point2_connections = triangleRegion.getConnectedHighers(*pointManifolds.at(2));
   ASSERT_EQ(point2_connections.size(), 2);
   ASSERT_EQ(point2_connections.front(), edgeManifolds.at(2));
   ASSERT_EQ(point2_connections.back(), edgeManifolds.at(1));

   const auto point0_connections = triangleRegion.getConnectedHighers(*pointManifolds.at(0));
   ASSERT_EQ(point0_connections.size(), 2);
   ASSERT_EQ(dynamic_cast<const DirectedEdge2AsManifold1<double>*>(point0_connections.at(0))->getName(), "foo_edge_0");
   ASSERT_EQ(dynamic_cast<const DirectedEdge2AsManifold1<double>*>(point0_connections.at(1))->getName(), "foo_edge_2");

   const auto point1_connections = triangleRegion.getConnectedHighers(*pointManifolds.at(1));
   ASSERT_EQ(point1_connections.size(), 2);
   ASSERT_EQ(dynamic_cast<const DirectedEdge2AsManifold1<double>*>(point1_connections.at(0))->getName(), "foo_edge_1");
   ASSERT_EQ(dynamic_cast<const DirectedEdge2AsManifold1<double>*>(point1_connections.at(1))->getName(), "foo_edge_0");
}
