#include "TopologyDimension.h"
#include "IManifold.h"

namespace Geometry
{
   template<typename T, int N>
   class IManifold
   {
   public:
      virtual ~IManifold() noexcept = default;

      virtual TopologyDimension GetTopologyDimension() const = 0;
   };

}