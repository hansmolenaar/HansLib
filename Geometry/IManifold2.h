
#include "IManifold.h"
#include "Point.h"
#include "Hyperplane.h"

namespace Geometry
{
   class IManifold2 : public IManifold<3>
   {
   public:
      virtual Hyperplane<3> GetEuclideanSubspaceAt(const Point<double, 3>& point) const = 0;

      TopologyDimension GetTopologyDimension() const override { return TopologyDimensionDef::Face; };
   };

}