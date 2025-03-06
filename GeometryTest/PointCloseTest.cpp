#include <gtest/gtest.h>

#include "PointClose.h"

TEST(PointCloseTest, Int)
{
   PointClose<int, 2> areClose;
   const IntPoint2 p0{ 1,42 };
   const IntPoint2 p1{ 1,43 };
   ASSERT_FALSE(areClose(p0, p1));
   ASSERT_TRUE(areClose(p0, p0));
   auto tmp = areClose.samePoints(p0, p1);
   //ASSERT_TRUE(areClose.samePoints(p0, p1));
}

TEST(PointCloseTest, Double)
{
   PointClose<double, 3> areClose;
   const Point3 p0{ 1,42, 0 };
   const Point3 p1{ 1,42, 1 };
   ASSERT_FALSE(areClose(p0, p1));
   ASSERT_TRUE(areClose(p0, p0));
}