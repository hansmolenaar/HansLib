#pragma once

#include "IManifold1.h"
#include "DirectedEdgeIntersections.h"

namespace Geometry
{

   template<typename T>
   class IManifold1D2 : public IManifold1<T, 2>
   {
   public:
      static constexpr int GeomDim = 2;

      // If the edge is fully/partially contained by the manifold then return a directed edge
      // Throw if the edge is degenerate
      virtual DirectedEdgeIntersections<T, GeomDim> GetIntersections(const DirectedEdge<T, GeomDim>& edge, const IGeometryPredicate<T, GeomDim>& predicate) const = 0;
   };

};