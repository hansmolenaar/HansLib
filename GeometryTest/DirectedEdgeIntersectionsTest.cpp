#include <gtest/gtest.h>

#include "DirectedEdge.h"
#include "DirectedEdgeIntersections.h"
#include "PointClose.h"

using namespace Geometry;

TEST(DirectedEdgeIntersections, empty)
{
    const PointClose<double, GeomDim2> areClose;
    const std::vector<DirectedEdgePoint<double, GeomDim2>> intersectionPoints;
    const auto isContained = [](const Point2 &p) { return false; };
    const DirectedEdgeIntersections<double, GeomDim2> intersections(intersectionPoints, isContained, areClose);
    ASSERT_TRUE(intersections.get().empty());
}

TEST(DirectedEdgeIntersections, single)
{
    const PointClose<double, GeomDim2> areClose;
    const Point2 p0{0, 0};
    const Point2 p1{1, 1};
    const DirectedEdge<double, GeomDim2> edge(p0, p1);
    const DirectedEdgePoint<double, GeomDim2> dep(p1, edge, areClose);

    const std::vector<DirectedEdgePoint<double, GeomDim2>> intersectionPoints{dep, dep};
    const auto isContained = [](const Point2 &p) { return false; };
    const DirectedEdgeIntersections<double, GeomDim2> intersections(intersectionPoints, isContained, areClose);
    ASSERT_FALSE(intersections.get().empty());
    ASSERT_TRUE(intersections.get().size() == 1);
    const auto i0 = intersections[0];
    ASSERT_TRUE(i0.getIsolatedPoint().getPointType() == DirectedEdgePointType::Point1);
}

TEST(DirectedEdgeIntersections, two)
{
    const PointClose<double, GeomDim2> areClose;
    const Point2 p0{0, 0};
    const Point2 p1{1, 1};
    const DirectedEdge<double, GeomDim2> edge(p0, p1);
    const DirectedEdgePoint<double, GeomDim2> dep0(p0, edge, areClose);
    const DirectedEdgePoint<double, GeomDim2> dep1(p1, edge, areClose);

    const std::vector<DirectedEdgePoint<double, GeomDim2>> intersectionPoints{dep0, dep1, dep0, dep1};
    const auto isContainedFalse = [](const Point2 &p) { return false; };
    DirectedEdgeIntersections<double, GeomDim2> intersections(intersectionPoints, isContainedFalse, areClose);
    ASSERT_FALSE(intersections.get().empty());
    ASSERT_TRUE(intersections.get().size() == 2);
    ASSERT_TRUE(intersections[0].getIsolatedPoint().getPointType() == DirectedEdgePointType::Point0);
    ASSERT_TRUE(intersections[1].getIsolatedPoint().getPointType() == DirectedEdgePointType::Point1);

    const auto isContainedTrue = [](const Point2 &p) { return true; };
    intersections = DirectedEdgeIntersections<double, GeomDim2>(intersectionPoints, isContainedTrue, areClose);
    ASSERT_TRUE(intersections.get().size() == 1);
    ASSERT_FALSE(intersections[0].isIsolatedPoint());
    ASSERT_TRUE(intersections[0].getInterval().Point0.getPointType() == DirectedEdgePointType::Point0);
    ASSERT_TRUE(intersections[0].getInterval().Point1.getPointType() == DirectedEdgePointType::Point1);
}

TEST(DirectedEdgeIntersections, three)
{
    const PointClose<double, GeomDim2> areClose;
    const Point2 p0{0, 0};
    const Point2 p1{0.5, 0.5};
    const Point2 p2{1, 1};
    const DirectedEdge<double, GeomDim2> edge(p0, p2);
    const DirectedEdgePoint<double, GeomDim2> dep0(p0, edge, areClose);
    const DirectedEdgePoint<double, GeomDim2> dep1(p1, edge, areClose);
    const DirectedEdgePoint<double, GeomDim2> dep2(p2, edge, areClose);

    const std::vector<DirectedEdgePoint<double, GeomDim2>> intersectionPoints{dep0, dep1, dep0, dep1, dep2, dep1, dep2};
    const auto isContainedFalse = [](const Point2 &p) { return false; };
    DirectedEdgeIntersections<double, GeomDim2> intersections(intersectionPoints, isContainedFalse, areClose);
    ASSERT_TRUE(intersections.get().size() == 3);
    ASSERT_TRUE(intersections[0].getIsolatedPoint().getPointType() == DirectedEdgePointType::Point0);
    ASSERT_TRUE(intersections[1].getIsolatedPoint().getPointType() == DirectedEdgePointType::Inside);
    ASSERT_TRUE(intersections[2].getIsolatedPoint().getPointType() == DirectedEdgePointType::Point1);

    const auto isContainedTrue = [](const Point2 &p) { return true; };
    intersections = DirectedEdgeIntersections<double, GeomDim2>(intersectionPoints, isContainedTrue, areClose);
    ASSERT_TRUE(intersections.get().size() == 1);
    ASSERT_FALSE(intersections[0].isIsolatedPoint());
    ASSERT_TRUE(intersections[0].getInterval().Point0.getPointType() == DirectedEdgePointType::Point0);
    ASSERT_TRUE(intersections[0].getInterval().Point1.getPointType() == DirectedEdgePointType::Point1);

    const auto isContained01 = [&areClose](const Point2 &p) { return areClose(p, Point2{0.25, 0.25}); };
    intersections = DirectedEdgeIntersections<double, GeomDim2>(intersectionPoints, isContained01, areClose);
    ASSERT_TRUE(intersections.get().size() == 2);
    ASSERT_FALSE(intersections[0].isIsolatedPoint());
    ASSERT_TRUE(intersections[0].getInterval().Point0.getPointType() == DirectedEdgePointType::Point0);
    ASSERT_TRUE(intersections[0].getInterval().Point1.getPointType() == DirectedEdgePointType::Inside);
    ASSERT_TRUE(intersections[1].isIsolatedPoint());
    ASSERT_TRUE(intersections[1].getIsolatedPoint().getPointType() == DirectedEdgePointType::Point1);

    const auto isContained12 = [&areClose](const Point2 &p) { return areClose(p, Point2{0.75, 0.75}); };
    intersections = DirectedEdgeIntersections<double, GeomDim2>(intersectionPoints, isContained12, areClose);
    ASSERT_TRUE(intersections.get().size() == 2);
    ASSERT_TRUE(intersections[0].isIsolatedPoint());
    ASSERT_TRUE(intersections[0].getIsolatedPoint().getPointType() == DirectedEdgePointType::Point0);
    ASSERT_FALSE(intersections[1].isIsolatedPoint());
    ASSERT_TRUE(intersections[1].getInterval().Point0.getPointType() == DirectedEdgePointType::Inside);
    ASSERT_TRUE(intersections[1].getInterval().Point1.getPointType() == DirectedEdgePointType::Point1);
}

TEST(DirectedEdgeIntersections, erase)
{
    const PointClose<double, GeomDim2> areClose;
    const Point2 p0{0, 0};
    const Point2 p1{1, 1};
    const DirectedEdge<double, GeomDim2> edge(p0, p1);
    const DirectedEdgePoint<double, GeomDim2> dep0(p0, edge, areClose);
    const DirectedEdgePoint<double, GeomDim2> dep1(p1, edge, areClose);

    const std::vector<DirectedEdgePoint<double, GeomDim2>> intersectionPoints{dep0, dep1, dep0, dep1};
    const auto isContainedFalse = [](const Point2 &p) { return false; };
    DirectedEdgeIntersections<double, GeomDim2> intersections(intersectionPoints, isContainedFalse, areClose);
    ASSERT_TRUE(intersections.get().size() == 2);
    ASSERT_TRUE(intersections.get().front().getIsolatedPoint().getPointType() == DirectedEdgePointType::Point0);
    intersections.erase(0);
    ASSERT_TRUE(intersections.get().size() == 1);
    ASSERT_TRUE(intersections.get().front().getIsolatedPoint().getPointType() == DirectedEdgePointType::Point1);
}
