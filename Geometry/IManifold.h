#pragma once

#include "IGeometryObject.h"
#include "IGeometryPredicate.h"
#include "TopologyDefines.h"

namespace Geometry
{
   template<typename T, int N>
   class IManifold : public IGeometryObject<T, N>
   {
   public:
      virtual bool contains(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const = 0;
      virtual Topology::TopologyDimension getTopologyDimension() const = 0;
      virtual const std::string& getName() const = 0;
   };

}
