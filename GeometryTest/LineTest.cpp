#include <gtest/gtest.h>

#include "Line.h"
#include "PointClose.h"

#include <optional>

using namespace Geometry;

TEST(LineTest, Project)
{
    const PointClose<double, 2> areClose;
    const Point2 point{1, 2};
    const UnitVector<double, 2> direction = UnitVector<double, 2>::Create(Point2{1, 0});
    const Line<double, 2> line(point, direction);
    ASSERT_TRUE(areClose(line.project(Point2{0, 0}), Point2{0, 2}));
    ASSERT_TRUE(areClose(line.project(Point2{10, 11}), Point2{10, 2}));
}

TEST(LineTest, Contains)
{
    const PointClose<double, 2> areClose;
    const Point2 point{1, 2};
    const UnitVector<double, 2> direction = UnitVector<double, 2>::Create(Point2{3, 4});
    const Line<double, 2> line(point, direction);
    ASSERT_TRUE(line.contains(Point2{-5, -6}, areClose));
    ASSERT_FALSE(line.contains(Point2{-5, -6.1}, areClose));
}

TEST(LineTest, TwoPointsConstructor)
{
    const PointClose<double, 2> areClose;
    const Point2 point1{1, 2};
    const Point2 point2{2, 1};
    const Line<double, 2> line(point1, point2);
    ASSERT_TRUE(line.contains(Point2{1.5, 1.5}, areClose));
}

TEST(LineTest, TwoPointsConstructor_error)
{
    const PointClose<double, 2> areClose;
    const Point2 point1{1, 2};
    const Point2 point2{2, 1};
    auto line = Line<double, 2>(point1, point2);
    ASSERT_MYEXCEPTION_MESSAGE((line = Line<double, 2>(point1, point1)), "Line<T, N>::Line invalid direction");
}