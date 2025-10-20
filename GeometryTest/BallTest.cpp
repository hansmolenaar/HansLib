#include <gtest/gtest.h>

#include "Ball.h"
#include "DirectedEdge.h"
#include "PointClose.h"
#include "PointExact.h"
#include "Rotate2D.h"

using namespace Geometry;

namespace
{
constexpr double crit = 1.0e-10;
}

TEST(BallTest, Constructor)
{
    const PointExact<int, GeomDim3> areClose;
    const IntPoint3 p{{2, -3, -6}};
    const Ball<int, GeomDim3> ball(p, 7);
    ASSERT_EQ(ball.getRadius(), 7);
    ASSERT_EQ(ball.getRadiusSquared(), 49);
    ASSERT_EQ(ball.getCenter().at(0), 2);
    ASSERT_EQ(ball.getCenter().at(1), -3);
    ASSERT_EQ(ball.getCenter().at(2), -6);
}

TEST(BallTest, GetPosition)
{
    const PointExact<int, GeomDim1> areClose;
    const Ball<int, GeomDim1> ball(IntPoint1{-2}, 7);
    ASSERT_EQ(ball.getPosition(IntPoint1{-8}, areClose), BallPosition::Inside);
    ASSERT_EQ(ball.getPosition(IntPoint1{-9}, areClose), BallPosition::On);
    ASSERT_EQ(ball.getPosition(IntPoint1{-10}, areClose), BallPosition::Outside);
}

TEST(BallTest, GetPositions)
{
    const PointExact<int, GeomDim1> areClose;
    const Ball<int, GeomDim1> ball(IntPoint1{-2}, 7);
    const DirectedEdge<int, GeomDim1> edge({5}, {6});
    const auto [pos0, pos1] = ball.getPositions(edge, areClose);
    ASSERT_EQ(pos0, BallPosition::On);
    ASSERT_EQ(pos1, BallPosition::Outside);
}

TEST(BallTest, Contains)
{
    const PointClose<double, GeomDim2> areClose;
    const Ball<double, GeomDim2> ball(Point2{1, 2}, 5);
    ASSERT_TRUE(ball.Contains(Point2{2, 2}, areClose));
    ASSERT_TRUE(ball.Contains(Point2{-2, 6}, areClose));
    ASSERT_FALSE(ball.Contains(Point2{-2, 7}, areClose));
}

TEST(BallTest, BoundingBox)
{
    const PointExact<int, GeomDim1> areClose;
    const Ball<int, GeomDim1> ball({2}, 5);
    const auto bb = ball.getBoundingBox();
    ASSERT_EQ(bb.getLower(0), -3);
    ASSERT_EQ(bb.getUpper(0), 7);
}

TEST(BallTest, FirstIntersectionFirstInside)
{
    const PointClose<double, GeomDim2> areClose;
    const Ball<double, GeomDim2> circle(Point2{0, 0}, 1.0);

    // Second Inside
    {
        const DirectedEdge<double, GeomDim2> edge(Point2{0.1, 0.2}, Point2{0.9, 0.3});
        const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
        ASSERT_TRUE(areClose.samePoints(*ip, edge.point1()));
    }

    // Second On
    {
        const DirectedEdge<double, GeomDim2> edge(Point2{0.1, 0.2}, Point2{-0.6, 0.8});
        const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
        ASSERT_TRUE(areClose.samePoints(*ip, edge.point1()));
    }

    // Second Outside
    {
        const DirectedEdge<double, GeomDim2> edge(Point2{0.1, 0.2}, Point2{1.1, 1.4});
        const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
        ASSERT_TRUE(areClose.samePoints(*ip, {0.6, 0.8}));
    }
}

TEST(BallTest, FirstIntersectionFirstOn)
{
    const PointClose<double, GeomDim2> areClose;
    const Ball<double, GeomDim2> circle(Point2{0, 0}, 1.0);
    const Point2 point0{0, 1};

    // Second Inside
    {
        const DirectedEdge<double, GeomDim2> edge(point0, Point2{0.9, 0.3});
        const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
        ASSERT_TRUE(areClose.samePoints(*ip, edge.point1()));
    }

    // Second On
    {
        const DirectedEdge<double, GeomDim2> edge(point0, Point2{1, 0});
        const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
        ASSERT_TRUE(areClose.samePoints(*ip, edge.point1()));
    }

    // Second Outside, no intersection
    {
        const DirectedEdge<double, GeomDim2> edge(point0, Point2{0, 2});
        const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
        ASSERT_FALSE(ip);
    }

    // Second Outside, edge case
    {
        const DirectedEdge<double, GeomDim2> edge(point0, Point2{1, 1});
        const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
        ASSERT_FALSE(ip);
    }

    // Second Outside, intersection
    {
        const DirectedEdge<double, GeomDim2> edge(point0, Point2{0, -2});
        const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
        ASSERT_TRUE(areClose.samePoints(*ip, Point2{0, -1}));
    }
}

TEST(BallTest, FirstIntersectionFirstOutside)
{
    const PointClose<double, GeomDim2> areClose;
    const Ball<double, GeomDim2> circle(Point2{0, 0}, 1.0);
    const Point2 point0{1, 1};
    const double sqrtHalf = std::sqrt(0.5);

    // Second Inside
    {
        const DirectedEdge<double, GeomDim2> edge(point0, Point2{0, 0});
        const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
        ASSERT_TRUE(areClose.samePoints(*ip, Point2{sqrtHalf, sqrtHalf}));
    }

    // Second On
    {
        const DirectedEdge<double, GeomDim2> edge(point0, Point2{1, 0});
        const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
        ASSERT_TRUE(areClose.samePoints(*ip, edge.point1()));
    }

    // Second Outside, no intersection
    {
        const DirectedEdge<double, GeomDim2> edge(point0, Point2{0, 2});
        const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
        ASSERT_FALSE(ip);
    }

    // Second Outside, edge case
    {
        const DirectedEdge<double, GeomDim2> edge(point0, Point2{-1, 1});
        const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
        ASSERT_TRUE(areClose.samePoints(*ip, Point2{0, 1}));
    }

    // Second Outside, intersection
    {
        const DirectedEdge<double, GeomDim2> edge(point0, Point2{-2, -2});
        const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
        ASSERT_TRUE(areClose.samePoints(*ip, Point2{sqrtHalf, sqrtHalf}));
    }
}

TEST(BallTest, FirstIntersection_1)
{
    const Ball<double, GeomDim2> ball(Point2{1.5, 2.5}, 3);
    const Point2 point0{4.78125, 5.78125};
    const Point2 point1{5.25, 6.25};
    const DirectedEdge<double, GeomDim2> edge(point0, point1);
    const PointClose<double, GeomDim2> predicate;
    const auto ip = ball.TryGetFirstIntersectionWithDirectedEdge(edge, predicate);
}
