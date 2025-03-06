#pragma once

#include "Defines.h"
#include "IGeometryRegion.h"

namespace Geometry
{
   template<typename T>
   class Polygon2AsRegion : public IGeometryRegion<T, GeomDim2>
   {

   public:
      Polygon2AsRegion(std::span<const Point<T, 2>> polygon, std::string name);

      bool contains(const Point<T, GeomDim2>& point, const IGeometryPredicate<T, GeomDim2>& predicate) const override;

      bool couldIntersectWith(typename const BoundingBox<T, GeomDim2>& bb, const IGeometryPredicate<T, GeomDim2>& predicate) const override;

      const IRegionManifolds<T, GeomDim2>& getManifolds() const override;

      BoundingBox<T, GeomDim2> getBoundingBox() const override;

      const std::string& getName() const;

   private:
      std::vector <Point<T, 2>>  m_polygon;
      std::string m_name;
   };

} // namespace Geometry