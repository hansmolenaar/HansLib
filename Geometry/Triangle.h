#pragma once

#include "Point.h"
#include "TriangleNodesOriented.h"
#include "IPointCollection.h"

namespace Triangle
{
   double AreaSigned(const Point2& p0, const Point2& p1, const Point2& p2);
   double SmallestAngle(const Point2& p0, const Point2& p1, const Point2& p2);
   std::array<Point2, Topology::NumNodesOnTriangle> getPoints(const Topology::TriangleNodesOriented& nodes, const IPointCollection<double, GeomDim2>& points);
   double getAreaSigned(const Topology::TriangleNodesOriented& nodes, const IPointCollection<double, GeomDim2>& points);
}
