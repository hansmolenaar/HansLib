#pragma once

#include "DirectedEdgeIntersection.h"
#include "IGeometryPredicate.h"

#include <functional>

namespace Geometry
{
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

      void erase(size_t n);

   private:
      std::vector<DirectedEdgeIntersection<T, N>> m_data;
   };

}