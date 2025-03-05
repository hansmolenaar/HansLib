#pragma once

#include "DirectedEdgeInterval.h"
#include "DirectedEdgePoint.h"

#include <variant>

namespace Geometry
{
   template<typename T, int N>
   class DirectedEdgeIntersection
   {
   public:
      DirectedEdgeIntersection<T, N>(DirectedEdgePoint<T, N> point);
      DirectedEdgeIntersection<T, N>(DirectedEdgeInterval<T, N> interval);
      bool isIsolatedPoint() const;
      const DirectedEdgePoint<T, N>& getPoint() const;
      const DirectedEdgeInterval<T, N>& getInterval() const;
   private:
      std::variant<DirectedEdgePoint<T, N>, DirectedEdgeInterval<T, N>> m_intersection;
   };

   template<typename T, int N>
   DirectedEdgeIntersection<T, N>::DirectedEdgeIntersection(DirectedEdgePoint<T, N> point) :
      m_intersection(std::move(point))
   {
   }

   template<typename T, int N>
   DirectedEdgeIntersection<T, N>::DirectedEdgeIntersection(DirectedEdgeInterval<T, N> interval) :
      m_intersection(std::move(interval))
   {
   }

   template<typename T, int N>
   bool DirectedEdgeIntersection<T, N>::isIsolatedPoint() const
   {
      return (std::holds_alternative<DirectedEdgePoint<T, N>>(m_intersection));
   }

   template<typename T, int N>
   const DirectedEdgePoint<T, N>& DirectedEdgeIntersection<T, N>::getPoint() const
   {
      return std::get<DirectedEdgePoint<T, N>>(m_intersection);
   }

   template<typename T, int N>
   const DirectedEdgeInterval<T, N>& DirectedEdgeIntersection<T, N>::getInterval() const
   {
      return std::get<DirectedEdgeInterval<T, N>>(m_intersection);
   }
}