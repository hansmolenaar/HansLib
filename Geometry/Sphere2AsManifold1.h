#pragma once

#include "Defines.h"
#include "Sphere.h"
#include "IManifold1D2.h"

namespace Geometry
{
   template<typename T>
   class Sphere2AsManifold1 : public IManifold1D2<T>
   {

   public:
      explicit  Sphere2AsManifold1(Sphere<T, GeomDim2> sphere);

      bool contains(const Point<T, GeomDim2>& point, const IGeometryPredicate<T, GeomDim2>& predicate) const override;

      Line<T, GeomDim2> GetEuclideanSubspaceAt(const Point<T, GeomDim2>& point, const IGeometryPredicate<T, GeomDim2>& predicate) const override;

      DirectedEdgeIntersections<T, GeomDim2> GetIntersections(const DirectedEdge<T, GeomDim2>& edge, const IGeometryPredicate<T, GeomDim2>& predicate) const override;

      BoundingBox<T, GeomDim2> getBoundingBox() const override;

   private:
      Sphere<T, GeomDim2> m_sphere;
   };

} // namespace Geometry