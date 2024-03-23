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
      static constexpr int GeometryDimension = 2;
      explicit  Sphere2AsManifold1(Sphere<T, GeometryDimension> sphere);

      bool contains(const Point<T, 2>& point, const IGeometryPredicate<T, 2>& predicate) const override;

      Line<T, 2> GetEuclideanSubspaceAt(const Point<T, 2>& point) const override;

      IntersectionDirectedEdges<T, 2> GetIntersectionsWithSimplex(const Simplex<T, 2>& simplex) const override;

   private:
      Sphere<T, GeometryDimension> m_sphere;
   };

} // namespace Geometry