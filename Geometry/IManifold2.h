#pragma once

#include "DirectedEdge.h"
#include "DirectedEdgeIntersections.h"
#include "Hyperplane.h"
#include "IManifold.h"
#include "Point.h"

namespace Geometry
{
   template<typename T>
   class IManifold2 : public IManifold<T, GeomDim3>
   {
   public:
      virtual Hyperplane<T, GeomDim3> getEuclideanSubspaceAt(const Point<T, GeomDim3>& point) const = 0;
      virtual DirectedEdgeIntersections<T, GeomDim3>  getIntersections(const DirectedEdge<T, GeomDim3>& edge, const IGeometryPredicate<T, GeomDim3>& predicate) const = 0;
      Topology::TopologyDimension getTopologyDimension() const override { return Topology::Face; };
   };
}