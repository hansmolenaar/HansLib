#include "DirectedEdge.h"
#include "DirectedEdgeIntersections.h"

using namespace Geometry;
template DirectedEdgeIntersections<double, 2>;

template<typename T, int N>
DirectedEdgeIntersections<T, N>::DirectedEdgeIntersections(
   std::span<const DirectedEdgePoint<T, N>> intersectionPoints,
   const std::function<bool(const Point<T, N>&)>& isContained,
   const std::function<bool(const Point<T, N>&, const Point<T, N>&)>& areSame)
{
   std::vector<DirectedEdgePoint<T, N>> ipoints(intersectionPoints.size());
   str::copy(intersectionPoints, ipoints.begin());
}

template<typename T, int N>
bool DirectedEdgeIntersections<T, N>::empty() const
{
   return m_data.empty();
}

template<typename T, int N>
size_t DirectedEdgeIntersections<T, N>::size() const
{
   return m_data.size();
}

template<typename T, int N>
const std::variant < DirectedEdgePoint<T, N>, DirectedEdgeInterval<T, N>>& DirectedEdgeIntersections<T, N>::operator[](size_t n) const
{
   return m_data.at(n);
}