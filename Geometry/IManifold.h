#include "TopologyDimension.h"
#include "IGeometryPredicate.h"

namespace Geometry
{
   template<typename T, int N>
   class IManifold
   {
   public:
      virtual ~IManifold() noexcept = default;
      virtual bool contains(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const = 0;
      virtual TopologyDimension GetTopologyDimension() const = 0;
   };

}
