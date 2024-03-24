#pragma once

#include "Defines.h"
#include "GeometrySphere.h"
#include "IManifold1D2.h"

namespace Geometry
{
   template<typename T>
   class Sphere2AsManifold1 : public IManifold1D2<T>
   {

   public:
      static constexpr int GeomDim = 2;
      explicit  Sphere2AsManifold1(Sphere<T, GeomDim> sphere);

      bool contains(const Point<T, GeomDim>& point, const IGeometryPredicate<T, GeomDim>& predicate) const override;

      Line<T, Sphere2AsManifold1<T>::GeomDim> GetEuclideanSubspaceAt(const Point<T, GeomDim>& point, const IGeometryPredicate<T, GeomDim>& predicate) const override;

      DirectedEdgeIntersections<T, 2> GetIntersections(const DirectedEdge<T, GeomDim>& edge, const IGeometryPredicate<T, GeomDim>& predicate) const override;

      BoundingBox<T, Sphere2AsManifold1<T>::GeomDim> getBoundingBox() const override;

   private:
      Sphere<T, GeomDim> m_sphere;
   };

} // namespace Geometry