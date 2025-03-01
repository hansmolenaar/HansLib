#pragma once

#include "DirectedEdge.h"
#include "GeometryDefines.h"
#include "IManifold1.h"

namespace Geometry
{

   template<typename T>
   class IManifold1D2 : public IManifold1<T, GeomDim2>
   {
   public:

      // Throw if the edge is degenerate
      virtual DirectedEdgeIntersections<T, GeomDim2> GetIntersections(const DirectedEdge<T, GeomDim2>& edge, const IGeometryPredicate<T, GeomDim2>& predicate) const = 0;
   };

};