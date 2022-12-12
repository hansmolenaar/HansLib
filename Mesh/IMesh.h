#pragma once

#include "Point/IPointCollection.h"

class ITopologicalAdjacencies;

template<typename T, int N>
class IMesh
{
public:
   virtual ~IMesh() = default;

   virtual const ITopologicalAdjacencies& getTopology() const = 0;
   virtual const IPointCollection<T, N>& getGeometry() const = 0;
};