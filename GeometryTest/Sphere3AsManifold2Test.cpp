#include <gtest/gtest.h>

#include "PointClose.h"
#include "Sphere3AsManifold2.h"

using namespace Geometry;
using namespace Topology;

namespace
{
   constexpr double crit = 1.0e-10;
}

TEST(Sphere3AsManifold2, Constructor)
{
   const PointClose<double, GeomDim3> areClose;
   const Point3 p{ {2,-3, 1} };
   const Sphere<double, GeomDim3> sphere(p, 7.0);
   const Sphere3AsManifold2<double> sphereManifold(sphere);

   //IManifold1D2TestInterface(sphereManifold, areClose);

   ASSERT_FALSE(sphereManifold.contains(p, areClose));
   const Point3 pointOnSphere{ 2,-3, -6 };
   ASSERT_TRUE(sphereManifold.contains(pointOnSphere, areClose));
   ASSERT_FALSE(sphereManifold.contains(Point3{ 4, 1, 8 }, areClose));
}

TEST(Sphere3AsManifold2, GetIntersections)
{
   const PointClose<double, GeomDim3> areClose;
   const Sphere<double, GeomDim3> sphere({ 0,0 }, 1.0);
   const Sphere3AsManifold2<double> sphereManifold(sphere);

   DirectedEdge<double, GeomDim3> edge({ 2,0, 0 }, { -2,0, 0 });

   auto intersections = sphereManifold.getIntersections(edge, areClose);
   ASSERT_EQ(intersections.get().size(), 2);
   auto ip = intersections[0].getIsolatedPoint().getPoint();
   ASSERT_TRUE(areClose(ip, Point3{ 1,0,0 }));
   ip = intersections[1].getIsolatedPoint().getPoint();
   ASSERT_TRUE(areClose(ip, Point3{ -1,0,0 }));
}
