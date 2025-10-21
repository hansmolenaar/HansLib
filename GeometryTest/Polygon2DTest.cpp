#include <gtest/gtest.h>

#include "PointClose.h"
#include "Polygon2D.h"
#include "Rational.h"

TEST(Polygon2DTest, SignedArea2D)
{
    const RatPoint2 p0{Rational{1, 1}, Rational{2, 1}};
    const RatPoint2 p1{Rational{3, 1}, Rational{2, 1}};
    const RatPoint2 p2{Rational{2, 1}, Rational{3, 1}};
    const std::vector<RatPoint2> trianglePos{p0, p1, p2};
    auto area = Polygon2D::AreaSigned<Rational>(trianglePos);
    ASSERT_EQ(Rational(1, 1), area);

    const std::vector<RatPoint2> triangleNeg{p1, p0, p2};
    area = Polygon2D::AreaSigned<Rational>(triangleNeg);
    ASSERT_EQ(Rational(-1, 1), area);
}

TEST(Polygon2DTest, Contains)
{
    const PointClose<double, 2> areClose;
    const Point2 p0{0, 0};
    const Point2 p1{1, 0};
    const Point2 p2{1, 1};
    const Point2 p3{0, 1};
    const std::vector<Point2> square{p0, p1, p2, p3};
    std::span<const Point<double, 2>> points = square;

    ASSERT_FALSE(Polygon2D::Contains(points, Point2{-1, -1}, areClose));
    ASSERT_FALSE(Polygon2D::Contains(points, Point2{-1, 0}, areClose));
    ASSERT_FALSE(Polygon2D::Contains(points, Point2{-1, 0.5}, areClose));
    ASSERT_FALSE(Polygon2D::Contains(points, Point2{-1, 1}, areClose));
    ASSERT_FALSE(Polygon2D::Contains(points, Point2{-1, 2}, areClose));

    ASSERT_FALSE(Polygon2D::Contains(points, Point2{0, -1}, areClose));
    ASSERT_TRUE(Polygon2D::Contains(points, Point2{0, 0}, areClose));
    ASSERT_TRUE(Polygon2D::Contains(points, Point2{0, 0.5}, areClose));
    ASSERT_TRUE(Polygon2D::Contains(points, Point2{0, 1}, areClose));
    ASSERT_FALSE(Polygon2D::Contains(points, Point2{0, 2}, areClose));

    ASSERT_FALSE(Polygon2D::Contains(points, Point2{0.5, -1}, areClose));
    ASSERT_TRUE(Polygon2D::Contains(points, Point2{0.5, 0}, areClose));
    ASSERT_TRUE(Polygon2D::Contains(points, Point2{0.5, 0.5}, areClose));
    ASSERT_TRUE(Polygon2D::Contains(points, Point2{0.5, 1}, areClose));
    ASSERT_FALSE(Polygon2D::Contains(points, Point2{0.5, 2}, areClose));

    ASSERT_FALSE(Polygon2D::Contains(points, Point2{1, -1}, areClose));
    ASSERT_TRUE(Polygon2D::Contains(points, Point2{1, 0}, areClose));
    ASSERT_TRUE(Polygon2D::Contains(points, Point2{1, 0.5}, areClose));
    ASSERT_TRUE(Polygon2D::Contains(points, Point2{1, 1}, areClose));
    ASSERT_FALSE(Polygon2D::Contains(points, Point2{1, 2}, areClose));

    ASSERT_FALSE(Polygon2D::Contains(points, Point2{2, -1}, areClose));
    ASSERT_FALSE(Polygon2D::Contains(points, Point2{2, 0}, areClose));
    ASSERT_FALSE(Polygon2D::Contains(points, Point2{2, 0.5}, areClose));
    ASSERT_FALSE(Polygon2D::Contains(points, Point2{2, 1}, areClose));
    ASSERT_FALSE(Polygon2D::Contains(points, Point2{2, 2}, areClose));
}
