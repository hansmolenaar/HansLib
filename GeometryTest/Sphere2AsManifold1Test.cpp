#include <gtest/gtest.h>

#include "PointClose.h"
#include "Sphere2AsManifold1.h"

using namespace Geometry;

namespace
{
   constexpr double crit = 1.0e-10;
}

TEST(Sphere2AsManifold1, Constructor)
{
   constexpr int geomdim = Sphere2AsManifold1<double>::GeometryDimension;
   const PointClose<double, geomdim> areClose;
   const Point2 p{ {2,-3} };
   const Sphere<double, geomdim> circle(p, 7.0);
   const Sphere2AsManifold1<double> sphereManifold(circle);
   ASSERT_EQ(sphereManifold.GetTopologyDimension(), TopologyDimensionDef::Edge);

   ASSERT_FALSE(sphereManifold.contains(p, areClose));
   const Point2 pointOnSphere{ 2,4 };
   ASSERT_TRUE(sphereManifold.contains(pointOnSphere, areClose));

   // TODO
   const  Line<double, 2> line = sphereManifold.GetEuclideanSubspaceAt(pointOnSphere);

   // TODO
   //IntersectionDirectedEdges<T, 2> Sphere2AsManifold1<T>::GetIntersectionsWithSimplex(const Simplex<T, 2>&simplex) const
}

