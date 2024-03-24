#pragma once

#include "IManifold.h"
#include "Line.h"
#include "Point.h"
#include "Simplex.h"
#include "DirectedEdgeIntersections.h"

namespace Geometry
{

   template<typename T, int N>
   class IManifold1 : public IManifold<T, N>
   {
   public:
      // Throw if point is not on manifold
      virtual Line<T, N> GetEuclideanSubspaceAt(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const = 0;

      TopologyDimension GetTopologyDimension() const override { return TopologyDimensionDef::Edge; };
   };

};