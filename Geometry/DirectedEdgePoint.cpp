#include "DirectedEdge.h"
#include "DirectedEdgePoint.h"

using namespace Geometry;

template DirectedEdgePoint<double, 2>;

template<typename T, int N>
DirectedEdgePoint<T, N>::DirectedEdgePoint()
{
   std::fill_n(m_edgePoint.data(), N, std::numeric_limits<T>::max());
}

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
T DirectedEdgePoint<T, N>::getScalar() const
{
   return m_scalar;
}