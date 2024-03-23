
#include "IManifold.h"
#include "Line.h"
#include "Point.h"
#include "Simplex.h"
#include "IntersectionDirectedEdges.h"

namespace Geometry
{

   template<typename T, int N>
   class IManifold1 : public IManifold<T, N>
   {
   public:
      virtual Line<T, N> GetEuclideanSubspaceAt(const Point<T, N>& point) const = 0;

      virtual IntersectionDirectedEdges<T, N> GetIntersectionsWithSimplex(const Simplex<T, N>& simplex) const = 0;

      TopologyDimension GetTopologyDimension() const override { return TopologyDimensionDef::Edge; };
   };

};