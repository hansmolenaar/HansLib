#pragma once

#include "Ball.h"
#include "IGeometryRegion.h"
#include "Sphere2AsManifold1.h"

namespace Geometry
{

   template<typename T>
   class Ball2AsRegion : public IGeometryRegion<T, 2>
   {
   public:
      static constexpr int GeomDim = 2;

      explicit Ball2AsRegion(Ball<T, 2> ball);

      BoundingBox<T, 2> getBoundingBox() const override;

      bool Contains(const Point<T, GeomDim>& point, const IGeometryPredicate<T, GeomDim>& predicate) const override;

      bool CouldIntersectWith(typename const BoundingBox<T, GeomDim>& bb, const IGeometryPredicate<T, GeomDim>& predicate) const override;

      const IRegionManifolds<T, 2>& getManifolds() const override;

   private:
      Ball<T, GeomDim> m_ball;
      std::unique_ptr<IRegionManifolds<T, 2>> m_sphereManifolds;
   };

} // namespace Geometry
