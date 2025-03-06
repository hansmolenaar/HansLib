#pragma once

#include "Defines.h"
#include "IManifold1D2.h"
#include "Sphere.h"

namespace Geometry
{
   template<typename T>
   class Sphere2AsManifold1 : public IManifold1D2<T>
   {

   public:
      explicit  Sphere2AsManifold1(Sphere<T, GeomDim2> sphere, std::string name = "Sphere2AsManifold1");

      bool contains(const Point<T, GeomDim2>& point, const IGeometryPredicate<T, GeomDim2>& predicate) const override;

      Line<T, GeomDim2> getEuclideanSubspaceAt(const Point<T, GeomDim2>& point, const IGeometryPredicate<T, GeomDim2>& predicate) const override;

      DirectedEdgeIntersections<T, GeomDim2> getIntersections(const DirectedEdge<T, GeomDim2>& edge, const IGeometryPredicate<T, GeomDim2>& predicate) const override;

      BoundingBox<T, GeomDim2> getBoundingBox() const override;

      const std::string& getName() const;

   private:
      Sphere<T, GeomDim2> m_sphere;
      std::string m_name;
   };

} // namespace Geometry