#include "Point.h"
#include "Polygon2D.h"
#include "Triangle.h"

#include <numbers>
#include <numeric>

double Triangle::AreaSigned(const Point2& p0, const Point2& p1, const Point2& p2)
{
   const std::array<Point2, 3> points{ p0, p1, p2 };
   return Polygon2D::AreaSigned<double>(points);
}

double Triangle::SmallestAngle(const Point2& p0, const Point2& p1, const Point2& p2)
{
   const double angle1 = PointUtils::Angle<double, 2>(p1, p0, p2);
   const double angle2 = PointUtils::Angle<double, 2>(p0, p1, p2);
   const double angle3 = std::numbers::pi - angle1 - angle2;
   const double minAngle = std::min(angle1, std::min(angle2, angle3));
   return minAngle;
}

std::array<Point2, Topology::NumNodesOnTriangle> Triangle::getPoints(const Topology::TriangleNodesOriented& nodes, const IPointCollection<double, GeomDim2>& points)
{
   return { points.getPoint(nodes[0]),points.getPoint(nodes[1]), points.getPoint(nodes[2]) };
}

double Triangle::getAreaSigned(const Topology::TriangleNodesOriented& nodes, const IPointCollection<double, GeomDim2>& points)
{
   const auto triangle = getPoints(nodes, points);
   return AreaSigned(triangle[0], triangle[1], triangle[2]);
}