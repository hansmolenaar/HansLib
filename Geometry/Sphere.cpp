#include "Sphere.h"

using namespace Geometry;

template class Sphere<double, 1>;
template class Sphere<double, 2>;
template class Sphere<double, 3>;

template <typename T, size_t N> Sphere<T, N>::Sphere(Point<T, N> center, T radius) : m_ball(std::move(center), radius)
{
}

template <typename T, size_t N> BoundingBox<T, N> Sphere<T, N>::getBoundingBox() const
{
    return m_ball.getBoundingBox();
}

template <typename T, size_t N>
bool Sphere<T, N>::Contains(const Point<T, N> &point, const IGeometryPredicate<T, N> &predicate) const
{
    return m_ball.getPosition(point, predicate) == BallPosition::On;
}

template <typename T, size_t N>
std::optional<Point<T, N>> Sphere<T, N>::TryGetFirstIntersectionWithDirectedEdge(
    const Geometry::DirectedEdge<T, N> &edge, const IGeometryPredicate<T, N> &predicate) const
{
    const auto &point0 = edge.point0();
    const auto &point1 = edge.point1();

    const auto [pos0, pos1] = m_ball.getPositions(edge, predicate);

    if (pos0 == BallPosition::Inside && pos1 == BallPosition::Inside)
        return {};
    if (pos0 == BallPosition::Inside && pos1 == BallPosition::On)
        return point1;
    if (pos0 == BallPosition::On && pos1 == BallPosition::Inside)
        return {};
    if (pos0 == BallPosition::On && pos1 == BallPosition::On)
        return point1;

    const auto ip = m_ball.TryGetFirstIntersectionWithDirectedEdge(edge, predicate);

    if (pos0 == BallPosition::Inside)
    {
        Utilities::MyAssert(pos1 == BallPosition::Outside);
        return *ip;
    }

    if (pos0 == BallPosition::On)
    {
        Utilities::MyAssert(pos1 == BallPosition::Outside);
        return ip;
    }

    Utilities::MyAssert(pos0 == BallPosition::Outside);
    return ip;
}

template <typename T, size_t N>
bool Sphere<T, N>::CouldIntersectWith(const BoundingBox<T, N> &bb, const IGeometryPredicate<T, N> &predicate) const
{
    if (!BoundingBox<T, N>::TryGetOverlap(getBoundingBox(), bb))
        return false;

    // If the whole bb is inside the sphere, there is no intersection
    if (m_ball.getPosition(bb.getLower(), predicate) != BallPosition::Inside)
        return true;
    if (m_ball.getPosition(bb.getUpper(), predicate) != BallPosition::Inside)
        return true;
    return false;
}

template <typename T, size_t N> Point<T, N> Sphere<T, N>::getCenter() const
{
    return m_ball.getCenter();
}

template <typename T, size_t N>
Geometry::DirectedEdgeIntersections<T, N> Sphere<T, N>::getIntersections(
    const DirectedEdge<T, N> &edge, const IGeometryPredicate<T, N> &predicate) const
{
    if (edge.isDegenerate(predicate))
    {
        throw MyException("Sphere<T, N>::getIntersections degenerate edge");
    }
    std::vector<DirectedEdgePoint<T, N>> intersectionPoints;
    const bool firstPointInside = Contains(edge.point0(), predicate);
    const auto firstIntersection = TryGetFirstIntersectionWithDirectedEdge(edge, predicate);

    if (firstPointInside)
    {
        intersectionPoints.emplace_back(edge.point0(), edge, predicate);
        if (firstIntersection)
        {
            intersectionPoints.emplace_back(firstIntersection.value(), edge, predicate);
        }
    }
    else if (firstIntersection.has_value())
    {
        intersectionPoints.emplace_back(firstIntersection.value(), edge, predicate);
        const DirectedEdge<T, N> next(firstIntersection.value(), edge.point1());
        if (!next.isDegenerate(predicate))
        {
            const auto secondIntersection = TryGetFirstIntersectionWithDirectedEdge(next, predicate);
            if (secondIntersection.has_value())
            {
                intersectionPoints.emplace_back(secondIntersection.value(), edge, predicate);
            }
        }
    }

    auto containsPoint = [this, &predicate](const Point<T, N> &p) { return Contains(p, predicate); };
    return DirectedEdgeIntersections<T, N>(intersectionPoints, containsPoint, predicate);
}
