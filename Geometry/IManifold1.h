
#include "IManifold.h"
#include "DirectedEdge.h"
#include "Point.h"


namespace Geometry
{
   template<int N>
   class IManifold1 : public IManifold<N>
   {
   public:
       virtual DirectedEdge<double, N> GetEuclideanSubspaceAt(const Point<double, N>& point) const = 0;

       TopologyDimension GetTopologyDimension() const override { return TopologyDimensionDef::Edge; };
   };

};