#include <gtest/gtest.h>

#include "Triangle.h"
#include <numbers>

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

TEST(TriangleTest, MinAngle)
{
   const Point2 p0{ 1.0, 1.0 };
   const Point2 p1{ 2.0, 1.0 };
   const Point2 p2{ 1.0, 1.0 + std::sqrt(3.0) };

   ASSERT_NEAR(Triangle::SmallestAngle(p0, p1, p2), std::numbers::pi / 6, 1.0e-12);
   ASSERT_NEAR(Triangle::SmallestAngle(p0, p2, p1), std::numbers::pi / 6, 1.0e-12);
   ASSERT_NEAR(Triangle::SmallestAngle(p1, p0, p2), std::numbers::pi / 6, 1.0e-12);
   ASSERT_NEAR(Triangle::SmallestAngle(p1, p2, p0), std::numbers::pi / 6, 1.0e-12);
   ASSERT_NEAR(Triangle::SmallestAngle(p2, p0, p1), std::numbers::pi / 6, 1.0e-12);
   ASSERT_NEAR(Triangle::SmallestAngle(p2, p1, p0), std::numbers::pi / 6, 1.0e-12);
}

TEST(TriangleTest, getCenter)
{
   const Point3 p0{ 1, 1, -2 };
   const Point3 p1{ 4, 1, 8 };
   const Point3 p2{ 10, 7, 3 };

   const Point3 center = Triangle::getCenter(p0, p1, p2);
   ASSERT_NEAR(center[0], 5.0, 1.0e-12);
   ASSERT_NEAR(center[1], 3.0, 1.0e-12);
   ASSERT_NEAR(center[2], 3.0, 1.0e-12);
}



TEST(TriangleTest, getAreaDirected)
{
   const Point3 p0{ 0, 0, 0 };
   const Point3 p1{ 1, 0, 0 };
   const Point3 p2{ 0, 1, 0 };

   const Point3 area = Triangle::getAreaDirected(p0, p1, p2);
   ASSERT_DOUBLE_EQ(area[0], 0.0);
   ASSERT_DOUBLE_EQ(area[1], 0.0);
   ASSERT_DOUBLE_EQ(area[2], 0.5);
}
