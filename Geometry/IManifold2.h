#pragma once

#include "IManifold.h"
#include "Point.h"
#include "Hyperplane.h"
#include "IntersectionDirectedEdges.h
#include "DirectedEdge.h"

namespace Geometry
{
   template<typename T>
   class IManifold2 : public IManifold<T, GeomDim3>
   {
   public:
      virtual Hyperplane<GeomDim3> GetEuclideanSubspaceAt(const Point<T, GeomDim3>& point) const = 0;
      virtual DirectedEdgeIntersections<T, GeomDim3>  GetIntersections(const DirectedEdge<T, GeomDim3>& edge) const = 0;
      TopologyDimension GetTopologyDimension() const override { return Topology::Face; };
   };

}