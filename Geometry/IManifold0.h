
#include "IManifold.h"
#include "Point.h"

namespace Geometry
{

   template<int N>
   class IManifold0 : public IManifold<N>
   {
   public:
      virtual Point<double, N> GetPoint() const = 0;

      TopologyDimension GetTopologyDimension() const override { return TopologyDimensionDef::Corner; };
   };

}