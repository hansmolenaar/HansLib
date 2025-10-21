#include "DirectedEdgeIntersections.h"
#include "DirectedEdge.h"

using namespace Geometry;

template class DirectedEdgeIntersections<double, 1>;
template class DirectedEdgeIntersections<double, 2>;
template class DirectedEdgeIntersections<double, 3>;

template <typename T, size_t N>
DirectedEdgeIntersections<T, N>::DirectedEdgeIntersections(const DirectedEdgeIntersection<T, N> &intersection)
    : m_data(1, intersection)
{
}

template <typename T, size_t N>
DirectedEdgeIntersections<T, N>::DirectedEdgeIntersections(std::span<const DirectedEdgePoint<T, N>> intersectionPoints,
                                                           const std::function<bool(const Point<T, N> &)> &isContained,
                                                           const IGeometryPredicate<T, N> &predicate)
{
    if (intersectionPoints.empty())
        return;

    std::vector<DirectedEdgePoint<T, N>> points(intersectionPoints.size());
    str::copy(intersectionPoints, points.begin());
    // Sort on scalar
    std::sort(points.begin(), points.end(), DirectedEdgePointLess{predicate});
    // Remove duplicates
    points.erase(std::unique(points.begin(), points.end(), DirectedEdgePointEquals{predicate}), points.end());

    auto addIntersection = [this, &points](size_t f, size_t l) {
        if (f == l) // Single point
        {
            m_data.push_back(DirectedEdgeIntersection<T, N>(points.at(f)));
        }
        else
        {
            m_data.push_back(DirectedEdgeInterval<T, N>{points.at(f), points.at(l)});
        }
    };
    size_t first = 0;
    size_t last = 0;
    while (first < points.size())
    {
        ++last;
        if (last >= points.size())
        {
            addIntersection(first, last - 1);
            break;
        }
        const auto mid = (points.at(last - 1).getPoint() + points.at(last).getPoint()) * 0.5;
        if (!isContained(mid))
        {
            addIntersection(first, last - 1);
            first = last;
        }
    }
}

template <typename T, size_t N>
const DirectedEdgeIntersection<T, N> &DirectedEdgeIntersections<T, N>::operator[](size_t n) const
{
    return m_data.at(n);
}

template <typename T, size_t N>
std::span<const DirectedEdgeIntersection<T, N>> DirectedEdgeIntersections<T, N>::get() const
{
    return m_data;
}

template <typename T, size_t N> void DirectedEdgeIntersections<T, N>::erase(size_t n)
{
    m_data.erase(m_data.begin() + n);
}
