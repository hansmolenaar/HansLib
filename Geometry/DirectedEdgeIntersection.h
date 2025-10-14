#pragma once

#include "DirectedEdgeInterval.h"
#include "DirectedEdgePoint.h"

#include <variant>

namespace Geometry
{
   template<typename T, size_t N>
   class DirectedEdgeIntersection
   {
   public:
      DirectedEdgeIntersection(DirectedEdgePoint<T, N> point);
      DirectedEdgeIntersection(DirectedEdgeInterval<T, N> interval);
      bool isIsolatedPoint() const;
      const DirectedEdgePoint<T, N>& getIsolatedPoint() const;
      const DirectedEdgeInterval<T, N>& getInterval() const;
   private:
      std::variant<DirectedEdgePoint<T, N>, DirectedEdgeInterval<T, N>> m_intersection;
   };

   template<typename T, size_t N>
   DirectedEdgeIntersection<T, N>::DirectedEdgeIntersection(DirectedEdgePoint<T, N> point) :
      m_intersection(std::move(point))
   {
   }

   template<typename T, size_t N>
   DirectedEdgeIntersection<T, N>::DirectedEdgeIntersection(DirectedEdgeInterval<T, N> interval) :
      m_intersection(std::move(interval))
   {
   }

   template<typename T, size_t N>
   bool DirectedEdgeIntersection<T, N>::isIsolatedPoint() const
   {
      return (std::holds_alternative<DirectedEdgePoint<T, N>>(m_intersection));
   }

   template<typename T, size_t N>
   const DirectedEdgePoint<T, N>& DirectedEdgeIntersection<T, N>::getIsolatedPoint() const
   {
      if (!isIsolatedPoint())
      {
         throw MyException("DirectedEdgeIntersection<T, N>::getIsolatedPoint()");
      }
      return std::get<DirectedEdgePoint<T, N>>(m_intersection);
   }

   template<typename T, size_t N>
   const DirectedEdgeInterval<T, N>& DirectedEdgeIntersection<T, N>::getInterval() const
   {
      if (isIsolatedPoint())
      {
         throw MyException("DirectedEdgeIntersection<T, N>::getInterval()");
      }
      return std::get<DirectedEdgeInterval<T, N>>(m_intersection);
   }
}
