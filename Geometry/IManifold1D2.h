#pragma once

#include "IManifold1.h"
#include "GeometryDefines.h"

namespace Geometry
{

   template<typename T>
   class IManifold1D2 : public IManifold1<T, GeomDim2>
   {
   public:

      // If the edge is fully/partially contained by the manifold then return a directed edge
      // Throw if the edge is degenerate
      virtual DirectedEdgeIntersections<T, GeomDim2> GetIntersections(const DirectedEdge<T, GeomDim2>& edge, const IGeometryPredicate<T, GeomDim2>& predicate) const = 0;
   };

};