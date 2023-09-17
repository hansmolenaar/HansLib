#include <gtest/gtest.h>

#include "GeometryBall.h"
#include "PointClose.h"
#include "PointExact.h"
#include "Rotate2D.h"
#include "DirectedEdge.h"

using namespace Geometry;

namespace
{
   constexpr double crit = 1.0e-10;
}

TEST(BallTest, Constructor)
{
   constexpr int geomdim = 3;
   const PointExact<int, geomdim> areClose;
   const IntPoint3 p{ {2,-3,-6} };
   const Ball<int, geomdim> ball(p, 7);
   ASSERT_EQ(ball.getRadius(), 7);
   ASSERT_EQ(ball.getRadiusSquared(), 49);
   ASSERT_EQ(ball.getCenter().at(0), 2);
   ASSERT_EQ(ball.getCenter().at(1), -3);
   ASSERT_EQ(ball.getCenter().at(2), -6);
}


TEST(BallTest, GetPosition)
{
   constexpr int geomdim = 1;
   const PointExact<int, geomdim> areClose;
   const Ball<int, geomdim> ball(IntPoint1{ -2 }, 7);
   ASSERT_EQ(ball.getPosition(IntPoint1{ -8 }, areClose), BallPosition::Inside);
   ASSERT_EQ(ball.getPosition(IntPoint1{ -9 }, areClose), BallPosition::On);
   ASSERT_EQ(ball.getPosition(IntPoint1{ -10 }, areClose), BallPosition::Outside);
}


TEST(BallTest, GetPositions)
{
   constexpr int geomdim = 1;
   const PointExact<int, geomdim> areClose;
   const Ball<int, geomdim> ball(IntPoint1{ -2 }, 7);
   const auto edge = DirectedEdge<int, geomdim>::Create({ 5 }, { 6 }, areClose);
   const auto [pos0, pos1] = ball.getPositions(edge);
   ASSERT_EQ(pos0, BallPosition::On);
   ASSERT_EQ(pos1, BallPosition::Outside);
}


TEST(BallTest, Contains)
{
   constexpr int geomdim = 2;
   const PointClose<double, geomdim> areClose;
   const Ball<double, geomdim> ball(Point2{ 1, 2 }, 5);
   ASSERT_TRUE(ball.Contains(Point2{ 2, 2 }, areClose));
   ASSERT_TRUE(ball.Contains(Point2{ -2, 6 }, areClose));
   ASSERT_FALSE(ball.Contains(Point2{ -2, 7 }, areClose));
}

#if false
TEST(SphereTest, Contains)
{
   constexpr int geomdim = 2;
   const PointClose<double, geomdim> areClose;
   const Sphere<double, geomdim> circle(Point2{ 0, 0 }, 5.0);
   const Point2 point{ 3,4 };
   ASSERT_TRUE(circle.Contains(point, areClose));

   for (int n = 0; n < 100; ++n)
   {
      const auto rotator = Rotate2D::Create(n);
      const auto pointRotated = (*rotator)(point);
      ASSERT_TRUE(circle.Contains(pointRotated, areClose));
   }
}

TEST(SphereTest, CouldIntersectWith)
{
   constexpr int geomdim = 1;
   const PointClose<double, geomdim> areClose;
   const Sphere<double, geomdim> circle(Point1{ 0 }, 1.0);
   auto bb = BoundingBox<double, geomdim>::CreateFromList(std::vector<Point1>{ {0.1}, { 1.1 }});
   ASSERT_TRUE(circle.CouldIntersectWith(bb, areClose));
}

TEST(SphereTest, FirstIntersectionInIn)
{
   constexpr int geomdim = 2;
   const PointClose<double, geomdim> areClose;
   const Sphere<double, geomdim> circle(Point2{ 0,0 }, 1.0);
   const auto edge = DirectedEdge<double, geomdim>::Create(Point2{ 0.1,0.2 }, Point2{ 0.9, 0.3 }, areClose);
   const auto [succes, intersect] = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
   ASSERT_FALSE(succes);
}

TEST(SphereTest, FirstIntersectionInOn)
{
   constexpr int geomdim = 2;
   const PointClose<double, geomdim> areClose;
   const Sphere<double, geomdim> circle(Point2{ 0,0 }, 1.0);
   const Point2 pointOn{ 0.6, 0.8 };
   const auto edge = DirectedEdge<double, geomdim>::Create(Point2{ 0.1,0.2 }, pointOn, areClose);
   const auto [succes, intersect] = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
   ASSERT_TRUE(succes);
   ASSERT_TRUE(areClose.SamePoints(intersect, pointOn));
}
#endif

