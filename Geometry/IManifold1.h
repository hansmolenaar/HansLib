
#include "IManifold.h"
#include "DirectedEdge.h"
#include "Point.h"
#inlcude "Simplex.h"
#include "IntersectionDirectedEdges.h"

#include  <boost/container/small_vector.hpp>

namespace Geometry
{

   template<typename T, int N>
   class IManifold1 : public IManifold<T, N>
   {
   public:
      virtual DirectedEdge<T, N> GetEuclideanSubspaceAt(const Point<T, N>& point) const = 0;

      IntersectionDirectedEdges<T, N> GetIntersectionsWithSimplex(const Simplex<T, N>& simplex) const = 0;

      TopologyDimension GetTopologyDimension() const override { return TopologyDimensionDef::Edge; };
   };

};