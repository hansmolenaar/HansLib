#include <gtest/gtest.h>

#include "Geometry/Rotate2D.h"
#include "Geometry/PointClose.h"

#include <numbers>

TEST(Rotate2DTest, Basic)
{
   const PointClose<double, 2> areClose;
   const Rotate2D rotate(std::numbers::pi/2);
   ASSERT_TRUE(areClose(rotate(Point2{ 1, 0 }), Point2{ 0, 1 }));
   ASSERT_TRUE(areClose(rotate(Point2{ 0, 1 }), Point2{ -1, 0 }));
   ASSERT_TRUE(areClose(rotate(Point2{ -1, 0 }), Point2{ 0, -1 }));
   ASSERT_TRUE(areClose(rotate(Point2{ 0, -1 }), Point2{ 1, 0 }));
}
