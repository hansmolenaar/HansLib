#include <gtest/gtest.h>

#include "IManifold1D2Test.h"
#include "PointClose.h"
#include "Polygon2AsManifold1.h"

using namespace Geometry;
using namespace Topology;

namespace
{
   constexpr double crit = 1.0e-10;
}

TEST(Polygon2AsManifold1, Base)
{
   const std::string name = "Polygon2AsManifold1_Constructor";
   const PointClose<double, GeomDim2> areClose;
   const Point2 p{ {2,-3} };
   const std::vector<Point2> trianglePoints{ Point2{0,0}, Point2{1,0}, Point2{0,1} };
   const Polygon2AsManifold1<double> triangleManifold(trianglePoints, name);

   IManifold1D2TestInterface(triangleManifold, areClose);

   ASSERT_FALSE(triangleManifold.contains(p, areClose));
   const std::vector<Point2> pointsInTriangle{ { 0.5, 0.5 }, {0.25, 0.25}, {1,0} };
   for (const auto& p : pointsInTriangle)
   {
      ASSERT_TRUE(triangleManifold.contains(p, areClose));
   }

   const auto bb = triangleManifold.getBoundingBox();
   ASSERT_TRUE(areClose(bb.getLower(), Point2{ 0, 0 }));
   ASSERT_TRUE(areClose(bb.getUpper(), Point2{ 1, 1 }));

   ASSERT_EQ(triangleManifold.getName(), name);

   const auto line = triangleManifold.GetEuclideanSubspaceAt({ 0.5, 0.5 }, areClose);
   ASSERT_TRUE(line.contains({ 1,0 }, areClose));
   ASSERT_TRUE(line.contains({ 0,1 }, areClose));
}

TEST(Polygon2AsManifold1, GetIntersections_1)
{
   const std::string name = "Polygon2AsManifold1_GetIntersections_1";
   const PointClose<double, GeomDim2> areClose;
   const std::vector<Point2> trianglePoints{ Point2{0,0}, Point2{1,0}, Point2{0,1} };
   const Polygon2AsManifold1<double> triangleManifold(trianglePoints, name);

   DirectedEdge<double, GeomDim2> edge({ -1, 0.5 }, { 2, 0.5 });
   triangleManifold.GetIntersections(edge, areClose);
}
