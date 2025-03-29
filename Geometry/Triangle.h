#pragma once

#include "IPointCollection.h"
#include "Point.h"
#include "TriangleNodesOriented.h"

namespace Triangle
{
   double AreaSigned(const Point2& p0, const Point2& p1, const Point2& p2);
   double SmallestAngle(const Point2& p0, const Point2& p1, const Point2& p2);
   std::array<Point2, Topology::NumNodesOnTriangle> getPoints(const Topology::TriangleNodesOriented& nodes, const IPointCollection2& points);
   double getAreaSigned(const Topology::TriangleNodesOriented& nodes, const IPointCollection2& points);

   Point3 getCenter(const Point3& p0, const Point3& p1, const Point3& p2);
   Point3 getCenter(const Topology::TriangleNodesOriented& nodes, const IPointCollection3& points);
   Point3 getAreaDirected(const Point3& p0, const Point3& p1, const Point3& p2);
   Point3 getAreaDirected(const Topology::TriangleNodesOriented& nodes, const IPointCollection3& points);
}
