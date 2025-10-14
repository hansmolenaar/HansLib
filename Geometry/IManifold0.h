#pragma once

#include "IManifold.h"
#include "Point.h"

namespace Geometry
{

   template<typename T, size_t N>
   class IManifold0 : public IManifold<T, N>
   {
   public:
      virtual Point<T, N> getPoint() const = 0;

      Topology::TopologyDimension getTopologyDimension() const override { return Topology::Corner; };
   };

}
