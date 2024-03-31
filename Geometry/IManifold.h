#pragma once

#include "TopologyDimension.h"
#include "IGeometryPredicate.h"
#include "IGeometryObject.h"

namespace Geometry
{
   template<typename T, int N>
   class IManifold : public IGeometryObject<T,N>
   {
   public:
      virtual bool contains(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const = 0;
      virtual Topology::TopologyDimension GetTopologyDimension() const = 0;
   };

}
