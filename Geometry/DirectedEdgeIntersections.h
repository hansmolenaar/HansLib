#pragma once

#include "DirectedEdgeIntersection.h"
#include "IGeometryPredicate.h"

#include <boost/container/static_vector.hpp>
#include <functional>

namespace Geometry
{
   // forward declaration
   template<typename T, int N>
   class DirectedEdge;

   template<typename T, int N>
   class DirectedEdgeIntersections
   {
   public:
      DirectedEdgeIntersections<T, N>() = default;
      DirectedEdgeIntersections<T, N>(
         std::span<const DirectedEdgePoint<T, N>> intersectionPoints,
         const std::function<bool(const Point<T, N>&)>& isContained,
         const IGeometryPredicate<T, N>& predicate);

      const DirectedEdgeIntersection<T, N>& operator[](size_t n) const;

      std::span<const DirectedEdgeIntersection<T, N>> get() const;

      // TODO
      //private:
      //boost::container::static_vector<DirectedEdgeIntersection<T, N>, 8 > m_data;
      std::vector<DirectedEdgeIntersection<T, N>> m_data;
   };

}