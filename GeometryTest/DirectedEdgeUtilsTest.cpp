#include <gtest/gtest.h>

#include "DirectedEdgeUtils.h"
#include "PointClose.h"

using namespace Geometry;

TEST(DirectedEdgeUtilsTest, degenerate)
{
   const PointClose<double, 2> areClose;
   const std::array<double, 2> point0{ 1, 2 };
   const std::array<double, 2> point1{ 2, 3 };
   const DirectedEdge<double, 2> edge00(point0, point0);
   const DirectedEdge<double, 2> edge01(point0, point1);
   ASSERT_MYEXCEPTION_MESSAGE(DirectedEdgeUtils::intersectWith2(edge00, edge01, areClose), "DirectedEdgeUtils::IntersectWith2 edge degenerate");
   const auto ipts = DirectedEdgeUtils::intersectWith2(edge01, edge00, areClose);
   ASSERT_TRUE(ipts->getIsolatedPoint().getPointType() == DirectedEdgePointType::Point0);
}

TEST(DirectedEdgeUtilsTest, isContained_0_or_1)
{
   const PointClose<double, 2> areClose;
   const std::array<double, 2> point0{ 1, 1 };
   const std::array<double, 2> point1{ 2, 2 };
   const std::array<double, 2> point2{ 1, 2 };
   const DirectedEdge<double, 2> edge01(point0, point1);

   auto ipts = DirectedEdgeUtils::intersectWith2(edge01, { point2, Point2{0,0} }, areClose);
   ASSERT_TRUE(!ipts);
   ipts = DirectedEdgeUtils::intersectWith2(edge01, { Point2{0,0}, point2 }, areClose);
   ASSERT_TRUE(!ipts);

   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point2, point0 }, areClose);
   ASSERT_TRUE(ipts->getIsolatedPoint().getPointType() == DirectedEdgePointType::Point0);
   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point0, point2 }, areClose);
   ASSERT_TRUE(ipts->getIsolatedPoint().getPointType() == DirectedEdgePointType::Point0);

   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point2, Point2{1.5,1.5} }, areClose);
   ASSERT_TRUE(ipts->getIsolatedPoint().getPointType() == DirectedEdgePointType::Inside);
   ipts = DirectedEdgeUtils::intersectWith2(edge01, { Point2{1.5,1.5}, point2 }, areClose);
   ASSERT_TRUE(ipts->getIsolatedPoint().getPointType() == DirectedEdgePointType::Inside);

   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point2, point1 }, areClose);
   ASSERT_TRUE(ipts->getIsolatedPoint().getPointType() == DirectedEdgePointType::Point1);
   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point1, point2 }, areClose);
   ASSERT_TRUE(ipts->getIsolatedPoint().getPointType() == DirectedEdgePointType::Point1);
   ASSERT_TRUE(ipts->getIsolatedPoint().getPointType() == DirectedEdgePointType::Point1);

   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point2, Point2{3,3} }, areClose);
   ASSERT_TRUE(!ipts);
   ipts = DirectedEdgeUtils::intersectWith2(edge01, { Point2{3,3}, point2 }, areClose);
   ASSERT_TRUE(!ipts);
}

TEST(DirectedEdgeUtilsTest, isContained_0_and_1)
{
   const PointClose<double, 2> areClose;
   const std::array<double, 2> point0{ 1, 1 };
   const std::array<double, 2> point1{ 2, 2 };

   const std::array<double, 2> point2{ -1, -1 };
   const std::array<double, 2> point3{ 0, 0 };
   const std::array<double, 2> point4{ 1.5, 1.5 };
   const std::array<double, 2> point5{ 3, 3 };
   const std::array<double, 2> point6{ 4, 4 };
   const DirectedEdge<double, 2> edge01(point0, point1);

   auto ipts = DirectedEdgeUtils::intersectWith2(edge01, { point2, point3 }, areClose);
   ASSERT_TRUE(!ipts);
   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point3, point2 }, areClose);
   ASSERT_TRUE(!ipts);

   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point2, point0 }, areClose);
   ASSERT_TRUE(ipts->getIsolatedPoint().getPointType() == DirectedEdgePointType::Point0);
   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point0, point2 }, areClose);
   ASSERT_TRUE(ipts->getIsolatedPoint().getPointType() == DirectedEdgePointType::Point0);

   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point4, point3 }, areClose);
   ASSERT_TRUE(ipts->getInterval().Point0.getPointType() == DirectedEdgePointType::Point0);
   ASSERT_TRUE(ipts->getInterval().Point1.getPointType() == DirectedEdgePointType::Inside);
   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point3, point4 }, areClose);
   ASSERT_TRUE(ipts->getInterval().Point0.getPointType() == DirectedEdgePointType::Point0);
   ASSERT_TRUE(ipts->getInterval().Point1.getPointType() == DirectedEdgePointType::Inside);

   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point4, point0 }, areClose);
   ASSERT_TRUE(ipts->getInterval().Point0.getPointType() == DirectedEdgePointType::Point0);
   ASSERT_TRUE(ipts->getInterval().Point1.getPointType() == DirectedEdgePointType::Inside);
   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point0, point4 }, areClose);
   ASSERT_TRUE(ipts->getInterval().Point0.getPointType() == DirectedEdgePointType::Point0);
   ASSERT_TRUE(ipts->getInterval().Point1.getPointType() == DirectedEdgePointType::Inside);

   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point1, point2 }, areClose);
   ASSERT_TRUE(ipts->getInterval().Point0.getPointType() == DirectedEdgePointType::Point0);
   ASSERT_TRUE(ipts->getInterval().Point1.getPointType() == DirectedEdgePointType::Point1);
   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point2, point1 }, areClose);
   ASSERT_TRUE(ipts->getInterval().Point0.getPointType() == DirectedEdgePointType::Point0);
   ASSERT_TRUE(ipts->getInterval().Point1.getPointType() == DirectedEdgePointType::Point1);

   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point1, point0 }, areClose);
   ASSERT_TRUE(ipts->getInterval().Point0.getPointType() == DirectedEdgePointType::Point0);
   ASSERT_TRUE(ipts->getInterval().Point1.getPointType() == DirectedEdgePointType::Point1);
   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point0, point1 }, areClose);
   ASSERT_TRUE(ipts->getInterval().Point0.getPointType() == DirectedEdgePointType::Point0);
   ASSERT_TRUE(ipts->getInterval().Point1.getPointType() == DirectedEdgePointType::Point1);

   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point1, point4 }, areClose);
   ASSERT_TRUE(ipts->getInterval().Point0.getPointType() == DirectedEdgePointType::Inside);
   ASSERT_TRUE(ipts->getInterval().Point1.getPointType() == DirectedEdgePointType::Point1);
   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point4, point1 }, areClose);
   ASSERT_TRUE(ipts->getInterval().Point0.getPointType() == DirectedEdgePointType::Inside);
   ASSERT_TRUE(ipts->getInterval().Point1.getPointType() == DirectedEdgePointType::Point1);

   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point5, point2 }, areClose);
   ASSERT_TRUE(ipts->getInterval().Point0.getPointType() == DirectedEdgePointType::Point0);
   ASSERT_TRUE(ipts->getInterval().Point1.getPointType() == DirectedEdgePointType::Point1);
   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point2, point5 }, areClose);
   ASSERT_TRUE(ipts->getInterval().Point0.getPointType() == DirectedEdgePointType::Point0);
   ASSERT_TRUE(ipts->getInterval().Point1.getPointType() == DirectedEdgePointType::Point1);

   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point5, point0 }, areClose);
   ASSERT_TRUE(ipts->getInterval().Point0.getPointType() == DirectedEdgePointType::Point0);
   ASSERT_TRUE(ipts->getInterval().Point1.getPointType() == DirectedEdgePointType::Point1);
   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point0, point5 }, areClose);
   ASSERT_TRUE(ipts->getInterval().Point0.getPointType() == DirectedEdgePointType::Point0);
   ASSERT_TRUE(ipts->getInterval().Point1.getPointType() == DirectedEdgePointType::Point1);

   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point5, point4 }, areClose);
   ASSERT_TRUE(ipts->getInterval().Point0.getPointType() == DirectedEdgePointType::Inside);
   ASSERT_TRUE(ipts->getInterval().Point1.getPointType() == DirectedEdgePointType::Point1);
   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point4, point5 }, areClose);
   ASSERT_TRUE(ipts->getInterval().Point0.getPointType() == DirectedEdgePointType::Inside);
   ASSERT_TRUE(ipts->getInterval().Point1.getPointType() == DirectedEdgePointType::Point1);

   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point5, point1 }, areClose);
   ASSERT_TRUE(ipts->getIsolatedPoint().getPointType() == DirectedEdgePointType::Point1);
   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point1, point5 }, areClose);
   ASSERT_TRUE(ipts->getIsolatedPoint().getPointType() == DirectedEdgePointType::Point1);

   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point5, point6 }, areClose);
   ASSERT_TRUE(!ipts);
   ipts = DirectedEdgeUtils::intersectWith2(edge01, { point6, point5 }, areClose);
   ASSERT_TRUE(!ipts);
}


TEST(DirectedEdgeUtilsTest, isContained_0_nor_1)
{
   const PointClose<double, 2> areClose;
   const std::array<double, 2> point0{ 1, 1 };
   const std::array<double, 2> point1{ 10, 10 };
   const DirectedEdge<double, 2> edge01(point0, point1);

   auto ipts = DirectedEdgeUtils::intersectWith2(edge01, { Point2{0,1}, Point2{0, 10} }, areClose);
   ASSERT_TRUE(!ipts);

   ipts = DirectedEdgeUtils::intersectWith2(edge01, { Point2{0,-10}, Point2{0, 10} }, areClose);
   ASSERT_TRUE(!ipts);

   ipts = DirectedEdgeUtils::intersectWith2(edge01, { Point2{1,-10}, Point2{1, 10} }, areClose);
   ASSERT_TRUE(ipts->getIsolatedPoint().getPointType() == DirectedEdgePointType::Point0);

   ipts = DirectedEdgeUtils::intersectWith2(edge01, { Point2{2,-10}, Point2{2, 10} }, areClose);
   ASSERT_TRUE(ipts->getIsolatedPoint().getPointType() == DirectedEdgePointType::Inside);

   ipts = DirectedEdgeUtils::intersectWith2(edge01, { Point2{10,-10}, Point2{10, 10} }, areClose);
   ASSERT_TRUE(ipts->getIsolatedPoint().getPointType() == DirectedEdgePointType::Point1);

   ipts = DirectedEdgeUtils::intersectWith2(edge01, { Point2{11,-10}, Point2{11, 10} }, areClose);
   ASSERT_TRUE(!ipts);

   ipts = DirectedEdgeUtils::intersectWith2(edge01, { Point2{6,4}, Point2{3, 7} }, areClose);
   ASSERT_TRUE(areClose.samePoints(ipts->getIsolatedPoint().getPoint(), Point2{ 5,5 }));
   ipts = DirectedEdgeUtils::intersectWith2(edge01, { Point2{3, 7}, Point2{6,4} }, areClose);
   ASSERT_TRUE(areClose.samePoints(ipts->getIsolatedPoint().getPoint(), Point2{ 5,5 }));

   ipts = DirectedEdgeUtils::intersectWith2(edge01, { Point2{3, 4}, Point2{9,7} }, areClose);
   ASSERT_TRUE(areClose.samePoints(ipts->getIsolatedPoint().getPoint(), Point2{ 5,5 }));

   ipts = DirectedEdgeUtils::intersectWith2(edge01, { Point2{0, 1}, Point2{6,4} }, areClose);
   ASSERT_TRUE(areClose.samePoints(ipts->getIsolatedPoint().getPoint(), Point2{ 2,2 }));
   ipts = DirectedEdgeUtils::intersectWith2(edge01, { Point2{6,4}, Point2{0, 1} }, areClose);
   ASSERT_TRUE(areClose.samePoints(ipts->getIsolatedPoint().getPoint(), Point2{ 2,2 }));
}