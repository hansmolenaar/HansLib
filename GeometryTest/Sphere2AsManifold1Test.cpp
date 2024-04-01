#include <gtest/gtest.h>

#include "PointClose.h"
#include "Sphere2AsManifold1.h"
#include "IManifold1D2Test.h"
#include "Single.h"

using namespace Geometry;
using namespace Topology;
using namespace Utilities;

namespace
{
   constexpr double crit = 1.0e-10;
}

TEST(Sphere2AsManifold1, Constructor)
{
   const PointClose<double, GeomDim2> areClose;
   const Point2 p{ {2,-3} };
   const Sphere<double, GeomDim2> circle(p, 7.0);
   const Sphere2AsManifold1<double> sphereManifold(circle);

   IManifold1D2TestInterface(sphereManifold, areClose);

   ASSERT_FALSE(sphereManifold.contains(p, areClose));
   const Point2 pointOnSphere{ 2,4 };
   ASSERT_TRUE(sphereManifold.contains(pointOnSphere, areClose));
}

TEST(Sphere2AsManifold1, GetEuclideanSubspaceAt)
{
   const PointClose<double, GeomDim2> areClose;
   const Point2 p{ 1,2 };
   const Sphere<double, GeomDim2> circle(p, 2.0);
   const Sphere2AsManifold1<double> sphereManifold(circle);

   const  Line<double, 2> line = sphereManifold.GetEuclideanSubspaceAt(Point2{ 3,2 }, areClose);
   ASSERT_TRUE(line.contains(Point2{ 3,0 }, areClose));

   ASSERT_THROW(sphereManifold.GetEuclideanSubspaceAt(Point2{ 3,3 }, areClose), MyException);
}

TEST(Sphere2AsManifold1, GetIntersections)
{
   const PointClose<double, GeomDim2> areClose;
   const Sphere<double, GeomDim2> circle({ 0,0 }, 2.0);
   const Sphere2AsManifold1<double> sphereManifold(circle);
   DirectedEdgeIntersections<double, GeomDim2> intersections;
   DirectedEdge<double, GeomDim2> edge({ 0,0 }, { 0,0 });
   DirectedEdgePoint2 ip;

   // First point outside
   {
      const Point2 first{ 0,4 };
      edge = { first, { 0,3 } };
      intersections = sphereManifold.GetIntersections(edge, areClose);
      ASSERT_EQ(intersections.size(), 0);

      edge = { first, { 0,2 } };
      intersections = sphereManifold.GetIntersections(edge, areClose);
      ASSERT_EQ(intersections.size(), 1);
       ip = std::get<DirectedEdgePoint2>(intersections[0]);
      ASSERT_EQ(ip.PointType, DirectedEdgePointType::Point1);
      ASSERT_TRUE(areClose.SamePoints(ip.EdgePoint, { 0,2 }));

      edge = { first, { 0,0 } };
      intersections = sphereManifold.GetIntersections(edge, areClose);
      ip = std::get<DirectedEdgePoint2>(intersections[0]);
      ASSERT_EQ(ip.PointType, DirectedEdgePointType:: Inside);
      ASSERT_TRUE(areClose.SamePoints(ip.EdgePoint, { 0,2 }));

      edge = { first, { 0,-2 } };
      intersections = sphereManifold.GetIntersections(edge, areClose);
      ASSERT_EQ(intersections.size(), 2);
      ip = std::get<DirectedEdgePoint2>(intersections[0]);
      ASSERT_EQ(ip.PointType, DirectedEdgePointType:: Inside);
      ASSERT_TRUE(areClose.SamePoints(ip.EdgePoint, { 0,2 }));
      ip = std::get<DirectedEdgePoint2>(intersections[1]);
      ASSERT_EQ(ip.PointType, DirectedEdgePointType::Point1);
      ASSERT_TRUE(areClose.SamePoints(ip.EdgePoint, { 0,-2 }));

      edge = { first, { 0,-3 } };
      intersections = sphereManifold.GetIntersections(edge, areClose);
      ASSERT_EQ(intersections.size(), 2);
      ip = std::get<DirectedEdgePoint2>(intersections[0]);
      ASSERT_EQ(ip.PointType, DirectedEdgePointType::Inside);
      ASSERT_TRUE(areClose.SamePoints(ip.EdgePoint, { 0,2 }));
      ip = std::get<DirectedEdgePoint2>(intersections[1]);
      ASSERT_EQ(ip.PointType, DirectedEdgePointType::Inside);
      ASSERT_TRUE(areClose.SamePoints(ip.EdgePoint, { 0,-2 }));
   }


   // First point on sphere
   {
      const Point2 first{ 0,2 };

      edge = { first, { 0,0 } };
      intersections = sphereManifold.GetIntersections(edge, areClose);
      ASSERT_EQ(intersections.size(), 1);
      ip = std::get<DirectedEdgePoint2>(intersections[0]);
      ASSERT_EQ(ip.PointType, DirectedEdgePointType::Point0);
      ASSERT_TRUE(areClose.SamePoints(ip.EdgePoint, { 0,2 }));

      edge = { first, { 0,-2 } };
      intersections = sphereManifold.GetIntersections(edge, areClose);
      ASSERT_EQ(intersections.size(), 2);
      ip = std::get<DirectedEdgePoint2>(intersections[0]);
      ASSERT_EQ(ip.PointType, DirectedEdgePointType::Point0);
      ASSERT_TRUE(areClose.SamePoints(ip.EdgePoint, { 0,2 }));
      ip = std::get<DirectedEdgePoint2>(intersections[1]);
      ASSERT_EQ(ip.PointType, DirectedEdgePointType::Point1);
      ASSERT_TRUE(areClose.SamePoints(ip.EdgePoint, { 0,-2 }));

      edge = { first, { 0,-3 } };
      intersections = sphereManifold.GetIntersections(edge, areClose);
      ASSERT_EQ(intersections.size(), 2);
      ip = std::get<DirectedEdgePoint2>(intersections[0]);
      ASSERT_EQ(ip.PointType, DirectedEdgePointType::Point0);
      ASSERT_TRUE(areClose.SamePoints(ip.EdgePoint, { 0,2 }));
      ip = std::get<DirectedEdgePoint2>(intersections[1]);
      ASSERT_EQ(ip.PointType, DirectedEdgePointType::Inside);
      ASSERT_TRUE(areClose.SamePoints(ip.EdgePoint, { 0,-2 }));
   }


   // First point in sphere
   {
      const Point2 first{ 0,0 };

      edge = { first, { 0,-1 } };
      intersections = sphereManifold.GetIntersections(edge, areClose);
      ASSERT_EQ(intersections.size(), 0);

      edge = { first, { 0,-2 } };
      intersections = sphereManifold.GetIntersections(edge, areClose);
      ASSERT_EQ(intersections.size(), 1);
      ip = std::get<DirectedEdgePoint2>(intersections[0]);
      ASSERT_EQ(ip.PointType, DirectedEdgePointType::Point1);
      ASSERT_TRUE(areClose.SamePoints(ip.EdgePoint, { 0,-2 }));

      edge = { first, { 0,-3 } };
      intersections = sphereManifold.GetIntersections(edge, areClose);
      ASSERT_EQ(intersections.size(), 1);
      ip = std::get<DirectedEdgePoint2>(intersections[0]);
      ASSERT_EQ(ip.PointType, DirectedEdgePointType::Inside);
      ASSERT_TRUE(areClose.SamePoints(ip.EdgePoint, { 0,-2 }));
   }

   // First point on sphere
   {
      const Point2 first{ 0,-2 };

      edge = { first, { 0,-3 } };
      intersections = sphereManifold.GetIntersections(edge, areClose);
      ASSERT_EQ(intersections.size(), 1);
      ip = std::get<DirectedEdgePoint2>(intersections[0]);
      ASSERT_EQ(ip.PointType, DirectedEdgePointType::Point0);
      ASSERT_TRUE(areClose.SamePoints(ip.EdgePoint, { 0,-2 }));
   }

   // All outside
   {
      const Point2 first{ 0,-3 };

      edge = { first, { 0,-4 } };
      intersections = sphereManifold.GetIntersections(edge, areClose);
      ASSERT_EQ(intersections.size(), 0);
   }
}