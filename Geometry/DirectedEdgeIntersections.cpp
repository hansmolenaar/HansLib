#include "DirectedEdge.h"
#include "DirectedEdgeIntersections.h"

using namespace Geometry;

template DirectedEdgePoint<double, 2>;
template DirectedEdgeIntersections<double, 2>;

template<typename T, int N>
DirectedEdgePoint<T, N>::DirectedEdgePoint(const Point<T, N>& point, const DirectedEdge<T, N>& edge, const IGeometryPredicate<T, N>& predicate) :
   m_scalar(edge.project(point)),
   m_edgePoint(point),
   m_pointType(DirectedEdgePointType::Inside)
{
   if (predicate.SamePoints(point, edge.point0()))
   {
      m_edgePoint = edge.point0();
      m_pointType = DirectedEdgePointType::Point0;
   }
   if (predicate.SamePoints(point, edge.point1()))
   {
      m_edgePoint = edge.point1();
      m_pointType = DirectedEdgePointType::Point1;
   }
}

template<typename T, int N>
DirectedEdgePointType DirectedEdgePoint<T, N>::getPointType() const
{
   return m_pointType;
}

template<typename T, int N>
const Point<T, N>& DirectedEdgePoint<T, N>::getPoint() const
{
   return m_edgePoint;
}

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