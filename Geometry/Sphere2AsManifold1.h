#pragma once

#include "Defines.h"
#include "GeometrySphere.h"
#include "IGeometryPredicate.h"
#include "IManifold1.h"


namespace Geometry
{
   template<typename T>
   class Sphere2AsManifold1 : public IManifold1<T, 2>
   {

   public:
      static constexpr int GeomDim = 2;
      explicit  Sphere2AsManifold1(Sphere<T, GeomDim> sphere);

      bool contains(const Point<T, GeomDim>& point, const IGeometryPredicate<T, GeomDim>& predicate) const override;

      Line<T, Sphere2AsManifold1<T>::GeomDim> GetEuclideanSubspaceAt(const Point<T, GeomDim>& point, const IGeometryPredicate<T, GeomDim>& predicate) const override;

      DirectedEdgeIntersections<T, 2> GetIntersections(const DirectedEdge<T, GeomDim>& edge, const IGeometryPredicate<T, GeomDim>& predicate) const override;

   private:
      Sphere<T, GeomDim> m_sphere;
   };

} // namespace Geometry