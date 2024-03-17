#include "TopologyDimension.h"

namespace Geometry
{
   template<int N>
   class IManifold
   {
   public:
      virtual ~IManifold() noexcept = default;
      virtual TopologyDimension GetTopologyDimension() const = 0;
   };

}