#include <gtest/gtest.h>

#include "Triangle.h"

TEST(TriangleTest, SignedArea2D)
{
   const Point2 p0{ 1.0, 2.0 };
   const Point2 p1{ 3.0, 2.0 };
   const Point2 p2{ 2.0, 3.0 };
   auto area = Triangle::AreaSigned(p0, p1, p2);
   ASSERT_NEAR(1.0, area, 1.0e-12);

   area = Triangle::AreaSigned(p1, p0, p2);
   ASSERT_NEAR(-1.0, area, 1.0e-12);
}