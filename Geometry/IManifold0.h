#pragma once

#include "IManifold.h"
#include "Point.h"

namespace Geometry
{

   template<typename T, int N>
   class IManifold0 : public IManifold<T, N>
   {
   public:
      virtual Point<T, N> GetPoint() const = 0;

      Topology::TopologyDimension getTopologyDimension() const override { return Topology::Corner; };
   };

}