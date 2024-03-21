#include <gtest/gtest.h>

#include "Rotate2D.h"
#include "PointClose.h"
#include "UnitVector.h"

#include <numbers>

TEST(Rotate2DTest, Basic)
{
   const PointClose<double, 2> areClose;
   const Rotate2D rotate(std::numbers::pi / 2);
   ASSERT_TRUE(areClose(rotate(Point2{ 1, 0 }), Point2{ 0, 1 }));
   ASSERT_TRUE(areClose(rotate(Point2{ 0, 1 }), Point2{ -1, 0 }));
   ASSERT_TRUE(areClose(rotate(Point2{ -1, 0 }), Point2{ 0, -1 }));
   ASSERT_TRUE(areClose(rotate(Point2{ 0, -1 }), Point2{ 1, 0 }));
}


TEST(Rotate2DTest, UnitVector)
{
   const Point2 p{ 1,0 };
   const auto uv = UnitVector<double, 2>::Create(Point2{ 1,0 });
   const Rotate2D rotate(std::numbers::pi / 2);
   const auto rotated = rotate(*uv);
   ASSERT_NEAR((rotated)[0], 0.0, 1.0e-10);
   ASSERT_NEAR((rotated)[1], 1.0, 1.0e-10);
}