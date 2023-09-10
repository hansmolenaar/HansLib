#include <gtest/gtest.h>

#include "PointExact.h"
#include "Rational.h"

TEST(PointExactTest, Int)
{
   PointExact<int, 2> areClose;
   const IntPoint2 p0{ 1,42 };
   const IntPoint2 p1{ 1,43 };
   ASSERT_FALSE(areClose(p0, p1));
   ASSERT_TRUE(areClose.SamePoints(p0, p0));
}

TEST(PointExactTest, Rational)
{
   PointExact<Rational, 1> areClose;
   const Point<Rational, 1> p0{ Rational{ 1, 42} };
   const Point<Rational, 1> p1{ Rational{ 1, 43} };
   ASSERT_FALSE(areClose(p0, p1));
   ASSERT_TRUE(areClose.SamePoints(p0, p0));
}