#pragma once

#include "Defines.h"
#include "IManifold1D2.h"

namespace Geometry
{
   template<typename T>
   class Polygon2AsManifold1 : public IManifold1D2<T>
   {

   public:
      Polygon2AsManifold1(std::span<const Point<T, 2>> polygon, std::string name);

      bool contains(const Point<T, GeomDim2>& point, const IGeometryPredicate<T, GeomDim2>& predicate) const override;

      Line<T, GeomDim2> GetEuclideanSubspaceAt(const Point<T, GeomDim2>& point, const IGeometryPredicate<T, GeomDim2>& predicate) const override;

      DirectedEdgeIntersections<T, GeomDim2> GetIntersections(const DirectedEdge<T, GeomDim2>& edge, const IGeometryPredicate<T, GeomDim2>& predicate) const override;

      BoundingBox<T, GeomDim2> getBoundingBox() const override;

      const std::string& getName() const;

   private:
      std::vector <Point<T, 2>>  m_polygon;
      std::string m_name;
   };

} // namespace Geometry