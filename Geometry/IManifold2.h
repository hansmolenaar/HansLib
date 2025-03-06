#pragma once

#include "DirectedEdge.h"
#include "Hyperplane.h"
#include "IManifold.h"
#include "IntersectionDirectedEdges.h
#include "Point.h"

namespace Geometry
{
   template<typename T>
   class IManifold2 : public IManifold<T, GeomDim3>
   {
   public:
      virtual Hyperplane<GeomDim3> GetEuclideanSubspaceAt(const Point<T, GeomDim3>& point) const = 0;
      virtual DirectedEdgeIntersections<T, GeomDim3>  GetIntersections(const DirectedEdge<T, GeomDim3>& edge) const = 0;
      TopologyDimension getTopologyDimension() const override { return Topology::Face; };
   };

}