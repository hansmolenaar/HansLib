#include "Polygon2D.h"
#include "DirectedEdge.h"
#include "MyAssert.h"
#include "MyException.h"
#include "Rational.h"

using namespace Utilities;
using namespace Geometry;

template double Polygon2D::AreaSigned<double>(std::span<const Point<double, 2>> polygon);
template Rational Polygon2D::AreaSigned<Rational>(std::span<const Point<Rational, 2>> polygon);

template bool Polygon2D::Contains(std::span<const Point<double, 2>> polygon, const Point<double, 2> &point,
                                  const IGeometryPredicate<double, 2> &predicate);

template <typename T> T Polygon2D::AreaSigned(std::span<const Point<T, 2>> polygon)
{
    T result = 0;
    const auto nPoints = polygon.size();
    if (nPoints < 3)
        throw MyException("Polygon2D::AreaSigned not enough points: " + std::to_string(nPoints));

    // The shoelace formula
    for (int n = 1; n < nPoints; ++n)
    {
        const auto &p1 = polygon[n - 1];
        const auto &p2 = polygon[n];
        result += (p1.at(0) * p2.at(1) - p2.at(0) * p1.at(1));
    }

    // Last edge
    const auto &p1 = polygon[nPoints - 1];
    const auto &p2 = polygon[0];

    result += (p1.at(0) * p2.at(1) - p2.at(0) * p1.at(1));
    result = result / 2;
    return result;
}

template <typename T>
bool Polygon2D::Contains(std::span<const Point<T, 2>> polygon, const Point<T, 2> &point,
                         const IGeometryPredicate<T, 2> &predicate)
{
    MyAssert(polygon.size() >= 3);

    bool inPolygon = false;

    for (size_t e = 0; e < polygon.size(); ++e)
    {
        const auto p0 = polygon[e];
        const auto p1 = polygon[(e + 1) % polygon.size()];
        const DirectedEdge edge(p0, p1);
        MyAssert(!edge.isDegenerate(predicate));

        // Check if point is right on edge
        if (edge.contains(point, predicate))
        {
            return true;
        }

        // Count intersection between with line in along X-axis
        if ((((p0.at(1) <= point[1]) && (point[1] < p1.at(1))) || ((p1.at(1) <= point[1]) && (point[1] < p0.at(1)))) &&
            (point[0] < (p1.at(0) - p0.at(0)) * (point[1] - p0.at(1)) / (p1.at(1) - p0.at(1)) + p0.at(0)))
        {
            inPolygon = !inPolygon;
        }
    }
    return inPolygon;
}
