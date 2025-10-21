#pragma once

#include "BoundingBox.h"
#include "Defines.h"
#include "IGeometryPredicate.h"
#include "Point.h"

namespace Geometry
{
template <typename T, size_t N> class DirectedEdge
{
  public:
    DirectedEdge(const Point<T, N> &from, const Point<T, N> &to);
    const Point<T, N> &point0() const
    {
        return m_from;
    }
    const Point<T, N> &point1() const
    {
        return m_to;
    }
    T lengthSquared() const;
    T project(const Point<T, N> &point) const;
    Point<T, N> interpolate(T lambda) const;
    bool contains(const Point<T, N> &point, const IGeometryPredicate<T, N> &predicate) const;
    bool isDegenerate(const IGeometryPredicate<T, N> &predicate) const;
    BoundingBox<T, N> getBoundingBox() const;
    // projected point is not necessarily on the edge (line segment)
    Point<T, N> projectPointOnLine(const Point<T, N> &point) const;

  private:
    Point<T, N> m_from;
    Point<T, N> m_to;
};

using DirectedEdge2 = DirectedEdge<double, GeomDim2>;

template <typename T, size_t N>
DirectedEdge<T, N>::DirectedEdge(const Point<T, N> &from, const Point<T, N> &to) : m_from(from), m_to(to)
{
}

template <typename T, size_t N> T DirectedEdge<T, N>::lengthSquared() const
{
    return PointUtils::GetNormSquared(m_to - m_from);
}

// 0 => point0
// 1 => point1
template <typename T, size_t N> Point<T, N> DirectedEdge<T, N>::interpolate(T lambda) const
{
    return point0() * (1 - lambda) + point1() * lambda;
}

template <typename T, size_t N> T DirectedEdge<T, N>::project(const Point<T, N> &point) const
{
    T inprod = 0;
    T norm2 = 0;
    for (int d = 0; d < N; ++d)
    {
        const T dif = (point1().at(d) - point0().at(d));
        inprod += dif * (point.at(d) - point0().at(d));
        norm2 += dif * dif;
    }
    const double norm2inv = 1.0 / norm2;
    if (!std::isfinite(norm2inv))
    {
        throw MyException("DirectedEdge<T, N>::project edge is degenerated");
    }

    return inprod / norm2;
}

template <typename T, size_t N> bool DirectedEdge<T, N>::isDegenerate(const IGeometryPredicate<T, N> &predicate) const
{
    return predicate.samePoints(point0(), point1());
}

template <typename T, size_t N>
bool DirectedEdge<T, N>::contains(const Point<T, N> &point, const IGeometryPredicate<T, N> &predicate) const
{
    // Project the point on the line
    const T lambda = project(point);

    // Projection within range of line?
    if (lambda >= 0 && lambda <= 1)
    {
        const auto projected = interpolate(lambda);
        return predicate.samePoints(projected, point);
    }
    else if (lambda < 0)
    {
        return predicate.samePoints(point0(), point);
    }
    else
    {
        return predicate.samePoints(point1(), point);
    }
}

template <typename T, size_t N> Point<T, N> DirectedEdge<T, N>::projectPointOnLine(const Point<T, N> &point) const
{
    const T lambda = project(point);
    return interpolate(lambda);
}

template <typename T, size_t N> BoundingBox<T, N> DirectedEdge<T, N>::getBoundingBox() const
{
    return BoundingBox<T, N>::CreateFrom2Points(m_from, m_to);
}

} // namespace Geometry
