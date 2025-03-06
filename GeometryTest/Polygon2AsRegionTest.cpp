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
   const std::string name = "Polygon2AsRegion_Constructor";
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

}
