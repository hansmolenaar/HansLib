#include "Triangle.h"
#include "Polygon2D.h"


double Triangle::AreaSigned(const Point2& p0, const Point2& p1, const Point2& p2)
{
	const std::array<Point2, 3> points{ p0, p1, p2 };
	return Polygon2D::AreaSigned<double>(points);
}