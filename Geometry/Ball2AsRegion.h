#pragma once

#include "Ball.h"
#include "IGeometryRegion.h"
#include "Sphere2AsManifold1.h"

namespace Geometry
{

   template<typename T>
   class Ball2AsRegion : public IGeometryRegion<T, GeomDim2>
   {
   public:

      Ball2AsRegion(Ball<T, GeomDim2> ball, std::string name);

      BoundingBox<T, GeomDim2> getBoundingBox() const override;

      bool contains(const Point<T, GeomDim2>& point, const IGeometryPredicate<T, GeomDim2>& predicate) const override;

      bool couldIntersectWith(const BoundingBox<T, GeomDim2>& bb, const IGeometryPredicate<T, GeomDim2>& predicate) const override;

      const IRegionManifolds<T, GeomDim2>& getManifolds() const override;

      const std::string& getName() const override;
   private:
      Ball<T, GeomDim2> m_ball;
      std::string m_name;
      std::unique_ptr<IRegionManifolds<T, GeomDim2>> m_sphereManifolds;
   };

} // namespace Geometry
