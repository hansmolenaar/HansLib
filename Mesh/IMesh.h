#pragma once

#include "IPointCollection.h"

namespace Topology
{
   class ITopologicalAdjacencies;
}

template<typename T, size_t N>
class IMesh
{
public:
   virtual ~IMesh() = default;

   virtual const Topology::ITopologicalAdjacencies& getTopology() const = 0;
   virtual const IPointCollection<T, N>& getGeometry() const = 0;
};
