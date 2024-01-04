#include "Triangle.h"
#include "Polygon2D.h"
#include "Point.h"

#include <numeric>
#include <numbers>

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