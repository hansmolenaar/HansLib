#include "IGeometryRegion.h"
#include "IModelFeature.h"
#include "Hyperplane.h"

#include <optional>

namespace Geometry
{
   template<typename T, int N>
   class IManifold : public IGeometryRegion<T, DimensionGlobal>
   {
   public:
      virtual ~IManifold() noexcept = default;

      // Name must be unique for sorting!
      virtual const IModelFeature& GetModelFeature() const = 0; 

      virtual std::optional<Point<T, N>> void MapPointToManifold(const Point<T, N>& point) const = 0;

      // The point should be on the manifold, but the behavior is undefined if it is not.
      virtual Hyperplane<N> GetEuclideanSpace(const Point<T, N>& point) const = 0;
   };

}