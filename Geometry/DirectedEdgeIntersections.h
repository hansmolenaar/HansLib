#pragma once

#include "IGeometryPredicate.h"
#include "IManifold.h" // TODO remove me

#include <boost/container/static_vector.hpp>
#include <functional>

namespace Geometry
{
   template<typename T, int N>
   class DirectedEdge;

   enum class DirectedEdgePointType { Point0, Inside, Point1 };

   template<typename T, int N>
   class DirectedEdgePoint
   {
   public:
      DirectedEdgePoint<T, N>() = default;

      DirectedEdgePoint<T, N>(const Point<T, N>& point, const DirectedEdge<T, N>& edge, const IGeometryPredicate<T, N>& predicate);
      DirectedEdgePointType getPointType() const;
      const Point<T, N>& getPoint() const;
   private:
      T m_scalar = std::numeric_limits<T>::max();
      Point<T, N> m_edgePoint;
      DirectedEdgePointType m_pointType;
   };

   using DirectedEdgePoint2 = DirectedEdgePoint<double, 2>;

   template<typename T, int N>
   struct DirectedEdgeInterval
   {
      DirectedEdgePoint<T, N> Point0;
      DirectedEdgePoint<T, N> Point1;
   };

   template<typename T, int N>
   class DirectedEdgeIntersections
   {
   public:
      DirectedEdgeIntersections<T, N>() = default;
      DirectedEdgeIntersections<T, N>(
         std::span<const DirectedEdgePoint<T, N>> intersectionPoints,
         const std::function<bool(const Point<T, N>&)>& isContained,
         const std::function<bool(const Point<T, N>&, const Point<T, N>&)>& areSame);

      bool empty() const;
      size_t size() const;
      const std::variant < DirectedEdgePoint<T, N>, DirectedEdgeInterval<T, N>>& operator[](size_t n) const;
      //private:
      boost::container::static_vector<std::variant < DirectedEdgePoint<T, N>, DirectedEdgeInterval<T, N>>, 8 > m_data;
   };
   //template<typename T, int N>
   //using DirectedEdgeIntersections = boost::container::static_vector< std::variant< DirectedEdgePoint<T, N>, DirectedEdgeInterval<T, N> >, 4>;

   //template<typename T, int N>
   //DirectedEdgeIntersections<T, N> CreateDirectedEdgeIntersections(std::span<const DirectedEdgePoint<T, N>> intersectionPoints, const IManifold<T, N>& manifold, const IGeometryPredicate<T, N>& predicate);
}