#include <gtest/gtest.h>

#include "GeometrySphere.h"
#include "PointClose.h"
#include "Rotate2D.h"

using namespace Geometry;

namespace
{
   constexpr double crit = 1.0e-10;
}

TEST(SphereTest, Constructor)
{
   constexpr int geomdim = 3;
   const PointClose<double, geomdim> areClose;
   const Point3 p{ {2,-3,-6} };
   const Sphere<double, geomdim> circle(p, 7.0);

   ASSERT_FALSE(circle.Contains(p, areClose));
   ASSERT_TRUE(circle.Contains({ 2, -3, 1 }, areClose));
}


TEST(SphereTest, Contains)
{
   constexpr int geomdim = 2;
   const PointClose<double, geomdim> areClose;
   const Sphere<double, geomdim> circle(Point2{ 0, 0 }, 5.0);
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
   constexpr int geomdim = 1;
   const PointClose<double, geomdim> areClose;
   const Sphere<double, geomdim> circle(Point1{ 0.5 }, 0.5);

   auto bb = BoundingBox<double, geomdim>::CreateFromList(std::vector<Point1>{ {-0.9}, { -0.1 }});
   ASSERT_FALSE(circle.CouldIntersectWith(bb, areClose));

   bb = BoundingBox<double, geomdim>::CreateFromList(std::vector<Point1>{ {-0.9}, { 0.1 }});
   ASSERT_TRUE(circle.CouldIntersectWith(bb, areClose));

   bb = BoundingBox<double, geomdim>::CreateFromList(std::vector<Point1>{ {-0.9}, { 1.1 }});
   ASSERT_TRUE(circle.CouldIntersectWith(bb, areClose));

   bb = BoundingBox<double, geomdim>::CreateFromList(std::vector<Point1>{ {0.1}, { 0.9 }});
   ASSERT_FALSE(circle.CouldIntersectWith(bb, areClose));

   bb = BoundingBox<double, geomdim>::CreateFromList(std::vector<Point1>{ {0.1}, { 1.1 }});
   ASSERT_TRUE(circle.CouldIntersectWith(bb, areClose));

   bb = BoundingBox<double, geomdim>::CreateFromList(std::vector<Point1>{ {1.1}, { 1.2 }});
   ASSERT_FALSE(circle.CouldIntersectWith(bb, areClose));
}


TEST(SphereTest, FirstIntersectionFirstInside)
{
   constexpr int geomdim = 2;
   const PointClose<double, geomdim> areClose;
   const Sphere<double, geomdim> circle(Point2{ 0,0 }, 1.0);
   const Point2 point0{ 0.1,0.2 };

   // Second Inside
   {
      const auto edge = DirectedEdge<double, geomdim>::Create(point0, Point2{ 0.9, 0.3 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_FALSE(ip);
   }

   // Second On
   {
      const auto edge = DirectedEdge<double, geomdim>::Create(point0, Point2{ -0.6, 0.8 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.SamePoints(*ip, edge.point1()));
   }

   // Second Outside
   {
      const auto edge = DirectedEdge<double, geomdim>::Create(point0, Point2{ 1.1, 1.4 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.SamePoints(*ip, { 0.6, 0.8 }));
   }
}



TEST(SphereTest, FirstIntersectionFirstOn)
{
   constexpr int geomdim = 2;
   const PointClose<double, geomdim> areClose;
   const Sphere<double, geomdim> circle(Point2{ 0,0 }, 1.0);
   const Point2 point0{ 0, 1 };

   // Second Inside
   {
      const auto edge = DirectedEdge<double, geomdim>::Create(point0, Point2{ 0.9, 0.3 });
      const auto  ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_FALSE(ip);
   }

   // Second On
   {
      const auto edge = DirectedEdge<double, geomdim>::Create(point0, Point2{ 1, 0 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.SamePoints(*ip, edge.point1()));
   }

   // Second Outside, no intersection
   {
      const auto edge = DirectedEdge<double, geomdim>::Create(point0, Point2{ 0, 2 });
      const auto  ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_FALSE(ip);
   }

   // Second Outside, edge case
   {
      const auto edge = DirectedEdge<double, geomdim>::Create(point0, Point2{ 1, 1 });
      const auto  ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_FALSE(ip);
   }

   // Second Outside, intersection
   {
      const auto edge = DirectedEdge<double, geomdim>::Create(point0, Point2{ 0, -2 });
      const auto  ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.SamePoints(*ip, Point2{ 0, -1 }));
   }
}


TEST(SphereTest, FirstIntersectionFirstOutside)
{
   constexpr int geomdim = 2;
   const PointClose<double, geomdim> areClose;
   const Sphere<double, geomdim> circle(Point2{ 0,0 }, 1.0);
   const Point2 point0{ 1, 1 };
   const double sqrtHalf = std::sqrt(0.5);

   // Second Inside
   {
      const auto edge = DirectedEdge<double, geomdim>::Create(point0, Point2{ 0, 0 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.SamePoints(*ip, Point2{ sqrtHalf, sqrtHalf }));
   }

   // Second On
   {
      const auto edge = DirectedEdge<double, geomdim>::Create(point0, Point2{ 1, 0 });
      const auto ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.SamePoints(*ip, edge.point1()));
   }

   // Second Outside, no intersection
   {
      const auto edge = DirectedEdge<double, geomdim>::Create(point0, Point2{ 0, 2 });
      const auto  ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_FALSE(ip);
   }

   // Second Outside, edge case
   {
      const auto edge = DirectedEdge<double, geomdim>::Create(point0, Point2{ -1, 1 });
      const auto  ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.SamePoints(*ip, Point2{ 0,1 }));
   }

   // Second Outside, intersection
   {
      const auto edge = DirectedEdge<double, geomdim>::Create(point0, Point2{ -2, -2 });
      const auto  ip = circle.TryGetFirstIntersectionWithDirectedEdge(edge, areClose);
      ASSERT_TRUE(areClose.SamePoints(*ip, Point2{ sqrtHalf, sqrtHalf }));
   }

}
