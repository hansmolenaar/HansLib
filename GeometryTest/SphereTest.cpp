#include <gtest/gtest.h>

#include "PointClose.h"
#include "Rotate2D.h"
#include "Sphere.h"

using namespace Geometry;

namespace
{
   constexpr double crit = 1.0e-10;
}

TEST(SphereTest, Constructor)
{
   const PointClose<double, GeomDim3> areClose;
   const Point3 p{ {2,-3,-6} };
   const Sphere<double, GeomDim3> circle(p, 7.0);

   ASSERT_FALSE(circle.Contains(p, areClose));
   ASSERT_TRUE(circle.Contains({ 2, -3, 1 }, areClose));
}


TEST(SphereTest, Contains)
{
   const PointClose<double, GeomDim2> areClose;
   const Sphere<double, GeomDim2> circle(Point2{ 0, 0 }, 5.0);
   const Point2 point{ 3,4 };
   ASSERT_TRUE(circle.Contains(point, areClose));

   for (int n = 0; n < 100; ++n)
   {
      const Rotate2D rotator(n);
      const auto pointRotated = rotator(point);
      ASSERT_TRUE(circle.Contains(pointRotated, areClose));
   }
}

TEST(SphereTest, CouldIntersectWith)
{
   const PointClose<double, GeomDim1> areClose;
   const Sphere<double, GeomDim1> circle(Point1{ 0.5 }, 0.5);

   auto bb = BoundingBox<double, GeomDim1>::CreateFromList(std::vector<Point1>{ {-0.9}, { -0.1 }});
   ASSERT_FALSE(circle.CouldIntersectWith(bb, areClose));

   bb = BoundingBox<double, GeomDim1>::CreateFromList(std::vector<Point1>{ {-0.9}, { 0.1 }});
   ASSERT_TRUE(circle.CouldIntersectWith(bb, areClose));

   bb = BoundingBox<double, GeomDim1>::CreateFromList(std::vector<Point1>{ {-0.9}, { 1.1 }});
   ASSERT_TRUE(circle.CouldIntersectWith(bb, areClose));

   bb = BoundingBox<double, GeomDim1>::CreateFromList(std::vector<Point1>{ {0.1}, { 0.9 }});
   ASSERT_FALSE(circle.CouldIntersectWith(bb, areClose));

   bb = BoundingBox<double, GeomDim1>::CreateFromList(std::vector<Point1>{ {0.1}, { 1.1 }});
   ASSERT_TRUE(circle.CouldIntersectWith(bb, areClose));

   bb = BoundingBox<double, GeomDim1>::CreateFromList(std::vector<Point1>{ {1.1}, { 1.2 }});
   ASSERT_FALSE(circle.CouldIntersectWith(bb, areClose));
}


TEST(SphereTest, FirstIntersectionFirstInside)
{
   const PointClose<double, GeomDim2> areClose;
   const Sphere<double, GeomDim2> circle(Point2{ 0,0 }, 1.0);
   const Point2 point0{ 0.1,0.2 };

   // Second Inside
   {
      const DirectedEdge<double, GeomDim2> edge(point0, Point2{ 0.9, 0.3 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_FALSE(ip);
   }

   // Second On
   {
      const DirectedEdge<double, GeomDim2> edge(point0, Point2{ -0.6, 0.8 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.samePoints(*ip, edge.point1()));
   }

   // Second Outside
   {
      const DirectedEdge<double, GeomDim2> edge(point0, Point2{ 1.1, 1.4 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.samePoints(*ip, { 0.6, 0.8 }));
   }
}



TEST(SphereTest, FirstIntersectionFirstOn)
{
   const PointClose<double, GeomDim2> areClose;
   const Sphere<double, GeomDim2> circle(Point2{ 0,0 }, 1.0);
   const Point2 point0{ 0, 1 };

   // Second Inside
   {
      const DirectedEdge<double, GeomDim2> edge(point0, Point2{ 0.9, 0.3 });
      const auto  ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_FALSE(ip);
   }

   // Second On
   {
      const DirectedEdge<double, GeomDim2> edge(point0, Point2{ 1, 0 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.samePoints(*ip, edge.point1()));
   }

   // Second Outside, no intersection
   {
      const DirectedEdge<double, GeomDim2> edge(point0, Point2{ 0, 2 });
      const auto  ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_FALSE(ip);
   }

   // Second Outside, edge case
   {
      const DirectedEdge<double, GeomDim2> edge(point0, Point2{ 1, 1 });
      const auto  ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_FALSE(ip);
   }

   // Second Outside, intersection
   {
      const  DirectedEdge<double, GeomDim2> edge(point0, Point2{ 0, -2 });
      const auto  ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.samePoints(*ip, Point2{ 0, -1 }));
   }
}


TEST(SphereTest, FirstIntersectionFirstOutside)
{
   const PointClose<double, GeomDim2> areClose;
   const Sphere<double, GeomDim2> circle(Point2{ 0,0 }, 1.0);
   const Point2 point0{ 1, 1 };
   const double sqrtHalf = std::sqrt(0.5);

   // Second Inside
   {
      const DirectedEdge<double, GeomDim2> edge(point0, Point2{ 0, 0 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.samePoints(*ip, Point2{ sqrtHalf, sqrtHalf }));
   }

   // Second On
   {
      const DirectedEdge<double, GeomDim2> edge(point0, Point2{ 1, 0 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.samePoints(*ip, edge.point1()));
   }

   // Second Outside, no intersection
   {
      const DirectedEdge<double, GeomDim2> edge(point0, Point2{ 0, 2 });
      const auto  ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_FALSE(ip);
   }

   // Second Outside, edge case
   {
      const DirectedEdge<double, GeomDim2> edge(point0, Point2{ -1, 1 });
      const auto  ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.samePoints(*ip, Point2{ 0,1 }));
   }

   // Second Outside, intersection
   {
      const DirectedEdge<double, GeomDim2> edge(point0, Point2{ -2, -2 });
      const auto  ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.samePoints(*ip, Point2{ sqrtHalf, sqrtHalf }));
   }
}

TEST(SphereTest, TryGetFirstIntersectionWithDirectedEdgeAll)
{
   const PointClose<double, GeomDim2> areClose;
   const Sphere<double, GeomDim2> circle(Point2{ 0,0 }, 1.0);
   Point2 point0{ 0, 0 };
   DirectedEdge<double, GeomDim2> edge(point0, point0);
   std::optional<Point2> ip;

   {
      point0 = { -3,0 };
      edge = { point0, {-2,0} };
      ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_FALSE(ip);

      edge = { point0, {-1,0} };
      ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.samePoints(ip.value(), { -1,0 }));

      edge = { point0, {0,0} };
      ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.samePoints(ip.value(), { -1,0 }));

      edge = { point0, {1,0} };
      ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.samePoints(ip.value(), { -1,0 }));

      edge = { point0, {2,0} };
      ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.samePoints(ip.value(), { -1,0 }));
   }

   {
      point0 = { -1,0 };
      edge = { point0, {-1,-1} };
      ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_FALSE(ip);

      edge = { point0, {0,0} };
      ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_FALSE(ip);

      edge = { point0, {1,0} };
      ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.samePoints(ip.value(), { 1,0 }));

      edge = { point0, {2,0} };
      ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.samePoints(ip.value(), { 1,0 }));
   }

   {
      point0 = { 0,0 };
      edge = { point0, {0.1,0.1} };
      ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_FALSE(ip);

      edge = { point0, {1,0} };
      ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.samePoints(ip.value(), { 1,0 }));

      edge = { point0, {2,0} };
      ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.samePoints(ip.value(), { 1,0 }));
   }

   {
      point0 = { 1,0 };
      edge = { point0, {2,0} };
      ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_FALSE(ip);
   }
}
