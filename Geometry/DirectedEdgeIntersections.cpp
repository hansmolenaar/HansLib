#include "DirectedEdge.h"
#include "DirectedEdgeIntersections.h"

using namespace Geometry;
template DirectedEdgeIntersections<double, 2>;

template<typename T, int N>
DirectedEdgeIntersections<T, N>::DirectedEdgeIntersections(
   std::span<const DirectedEdgePoint<T, N>> intersectionPoints,
   const std::function<bool(const Point<T, N>&)>& isContained,
   const IGeometryPredicate<T, N>& predicate)
{
   if (intersectionPoints.empty()) return;

   std::vector<DirectedEdgePoint<T, N>> points(intersectionPoints.size());
   str::copy(intersectionPoints, points.begin());
   // Sort on scalar
   std::sort(points.begin(), points.end(), DirectedEdgePointLess{ predicate });
   // Remove duplicates
   points.erase(std::unique(points.begin(), points.end(), DirectedEdgePointEquals{ predicate }), points.end());


   if (points.size() == 1)
   {
      m_data.push_back(points.front());
   }
   else
   {
      throw MyException("not yet implemented");
   }
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
const Geometry::DirectedEdgeIntersection<T, N>& DirectedEdgeIntersections<T, N>::operator[](size_t n) const
{
   return m_data.at(n);
}