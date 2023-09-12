#include <gtest/gtest.h>

#include "GeometrySphere.h"
#include "PointClose.h"
#include "Rotate2D.h"

using namespace Geometry;

namespace
{
   constexpr double crit = 1.0e-10;
}

TEST(SphereTest, Constructor)
{
   constexpr int geomdim = 3;
   const PointClose<double, geomdim> areClose;
   const Point3 p{ {2,-3,-6} };
   const Sphere<double, geomdim> circle(p, 7.0);

   ASSERT_FALSE(circle.Contains(p, areClose));
   ASSERT_TRUE(circle.Contains({ 2, -3, 1 }, areClose));
}


TEST(SphereTest, Contains)
{
   constexpr int geomdim = 2;
   const PointClose<double, geomdim> areClose;
   const Sphere<double, geomdim> circle(Point2{ 0, 0 }, 5.0);
   const Point2 point{ 3,4 };
   ASSERT_TRUE(circle.Contains(point, areClose));

   for (int n = 0; n < 100; ++n)
   {
      const auto rotator = Rotate2D::Create(n);
      const auto pointRotated = (*rotator)(point);
      ASSERT_TRUE(circle.Contains(pointRotated, areClose));
   }
}

