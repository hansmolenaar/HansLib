#pragma once

#include "IGeometryObject.h"
#include "IGeometryPredicate.h"
#include "TopologyDefines.h"

namespace Geometry
{
   template<typename T, int N>
   class IManifold;

   template<typename T, int N>
   std::strong_ordering operator<=>(const IManifold<T, N>& lhs, const IManifold<T, N>& rhs)
   {
      auto result = lhs.getTopologyDimension() <=> rhs.getTopologyDimension();
      if (result == std::strong_ordering::equal)
      {
         result = lhs.getName() <=> rhs.getName();
      }
      return result;
   }

   template<typename T, int N>
   bool operator==(const IManifold<T, N>& lhs, const IManifold<T, N>& rhs)
   {
      if (lhs.getTopologyDimension() != rhs.getTopologyDimension())
      {
         return false;
      }

      return  lhs.getName() == rhs.getName();
   }

   template<typename T, int N>
   class IManifold : public IGeometryObject<T, N>
   {
   public:
      virtual bool contains(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const = 0;
      virtual Topology::TopologyDimension getTopologyDimension() const = 0;
      virtual const std::string& getName() const = 0;

      friend std::strong_ordering operator<=><>(const IManifold<T, N>& lhs, const IManifold<T, N>& rhs);
      friend bool operator==<>(const IManifold<T, N>& lhs, const IManifold<T, N>& rhs);
   };


}
