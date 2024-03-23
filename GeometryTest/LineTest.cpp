#include <gtest/gtest.h>

#include "Line.h"
#include "PointClose.h"

using namespace Geometry;

TEST(LineTest, Project)
{
   const PointClose<double, 2> areClose;
   const Point2 point{ 1, 2 };
   const UnitVector<double, 2> direction = UnitVector<double, 2>::Create(Point2{ 1,0 }).value();
   const Line<double, 2> line(point, direction);
   ASSERT_TRUE(areClose(line.project(Point2{ 0, 0 }), Point2{ 0,2 }));
   ASSERT_TRUE(areClose(line.project(Point2{ 10, 11 }), Point2{ 10,2 }));
}


TEST(LineTest, Contains)
{
   const PointClose<double, 2> areClose;
   const Point2 point{ 1, 2 };
   const UnitVector<double, 2> direction = UnitVector<double, 2>::Create(Point2{ 3, 4 }).value();
   const Line<double, 2> line(point, direction);
   ASSERT_TRUE(line.contains(Point2{ -5, -6 }, areClose));
   ASSERT_FALSE(line.contains(Point2{ -5, -6.1 }, areClose));
}
