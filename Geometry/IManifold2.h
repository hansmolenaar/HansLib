
#include "IManifold.h"
#include "Point.h"
#include "Hyperplane.h"
#include "IntersectionDirectedEdges.h
#include "DirectedEdge.h"

namespace Geometry
{
   template<typename T>
   class IManifold2 : public IManifold<T, 3>
   {
   public:
      virtual Hyperplane<3> GetEuclideanSubspaceAt(const Point<T, 3>& point) const = 0;
      virtual IntersectionDirectedEdges<T, 3>  GetIntersections(const DirectedEdge<T, 3>& edge) const = 0;
      TopologyDimension GetTopologyDimension() const override { return TopologyDimensionDef::Face; };
   };

}