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
   const DirectedEdge<int, geomdim> edge({ 5 }, { 6 });
   const auto [pos0, pos1] = ball.getPositions(edge, areClose);
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


TEST(BallTest, BoundingBox)
{
   constexpr int geomdim = 1;
   const PointExact<int, geomdim> areClose;
   const Ball<int, geomdim> ball({ 2 }, 5);
   const auto bb = ball.getBoundingBox();
   ASSERT_EQ(bb.getLower(0), -3);
   ASSERT_EQ(bb.getUpper(0), 7);
}



TEST(BallTest, FirstIntersectionFirstInside)
{
   constexpr int geomdim = 2;
   const PointClose<double, geomdim> areClose;
   const Ball<double, geomdim> circle(Point2{ 0,0 }, 1.0);

   // Second Inside
   {
      const DirectedEdge<double, geomdim> edge(Point2{ 0.1,0.2 }, Point2{ 0.9, 0.3 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.SamePoints(*ip, edge.point1()));
   }

   // Second On
   {
      const DirectedEdge<double, geomdim> edge(Point2{ 0.1,0.2 }, Point2{ -0.6, 0.8 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.SamePoints(*ip, edge.point1()));
   }

   // Second Outside
   {
      const DirectedEdge<double, geomdim> edge(Point2{ 0.1, 0.2 }, Point2{ 1.1, 1.4 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.SamePoints(*ip, { 0.6, 0.8 }));
   }
}


TEST(BallTest, FirstIntersectionFirstOn)
{
   constexpr int geomdim = 2;
   const PointClose<double, geomdim> areClose;
   const Ball<double, geomdim> circle(Point2{ 0,0 }, 1.0);
   const Point2 point0{ 0, 1 };

   // Second Inside
   {
      const DirectedEdge<double, geomdim> edge(point0, Point2{ 0.9, 0.3 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.SamePoints(*ip, edge.point1()));
   }

   // Second On
   {
      const DirectedEdge<double, geomdim> edge(point0, Point2{ 1, 0 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.SamePoints(*ip, edge.point1()));
   }

   // Second Outside, no intersection
   {
      const DirectedEdge<double, geomdim> edge(point0, Point2{ 0, 2 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_FALSE(ip);
   }

   // Second Outside, edge case
   {
      const  DirectedEdge<double, geomdim> edge(point0, Point2{ 1, 1 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_FALSE(ip);
   }

   // Second Outside, intersection
   {
      const  DirectedEdge<double, geomdim> edge(point0, Point2{ 0, -2 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.SamePoints(*ip, Point2{ 0, -1 }));
   }
}

TEST(BallTest, FirstIntersectionFirstOutside)
{
   constexpr int geomdim = 2;
   const PointClose<double, geomdim> areClose;
   const Ball<double, geomdim> circle(Point2{ 0,0 }, 1.0);
   const Point2 point0{ 1, 1 };
   const double sqrtHalf = std::sqrt(0.5);

   // Second Inside
   {
      const DirectedEdge<double, geomdim> edge(point0, Point2{ 0, 0 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.SamePoints(*ip, Point2{ sqrtHalf, sqrtHalf }));
   }

   // Second On
   {
      const DirectedEdge<double, geomdim> edge(point0, Point2{ 1, 0 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.SamePoints(*ip, edge.point1()));
   }

   // Second Outside, no intersection
   {
      const DirectedEdge<double, geomdim> edge(point0, Point2{ 0, 2 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_FALSE(ip);
   }

   // Second Outside, edge case
   {
      const  DirectedEdge<double, geomdim> edge(point0, Point2{ -1, 1 });
      const auto  ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.SamePoints(*ip, Point2{ 0,1 }));
   }

   // Second Outside, intersection
   {
      const DirectedEdge<double, geomdim> edge(point0, Point2{ -2, -2 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.SamePoints(*ip, Point2{ sqrtHalf, sqrtHalf }));
   }

}


