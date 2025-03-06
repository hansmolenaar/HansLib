#pragma once

#include "Defines.h"
#include "DirectedEdge.h"
#include "IManifold1D2.h"
#include "Line.h"

namespace Geometry
{
   template<typename T>
   class DirectedEdge2AsManifold1 : public IManifold1D2<T>
   {

   public:
      explicit  DirectedEdge2AsManifold1(DirectedEdge<T, GeomDim2> edge, std::string name = "DirectedEdge2AsManifold1");

      bool contains(const Point<T, GeomDim2>& point, const IGeometryPredicate<T, GeomDim2>& predicate) const override;

      Line<T, GeomDim2> GetEuclideanSubspaceAt(const Point<T, GeomDim2>& point, const IGeometryPredicate<T, GeomDim2>& predicate) const override;

      DirectedEdgeIntersections<T, GeomDim2> GetIntersections(const DirectedEdge<T, GeomDim2>& edge, const IGeometryPredicate<T, GeomDim2>& predicate) const override;

      BoundingBox<T, GeomDim2> getBoundingBox() const override;

      const std::string& getName() const;

   private:
      std::string m_name;
      DirectedEdge<T, GeomDim2> m_edge;
      Line<T, GeomDim2> m_line;
      BoundingBox<T, GeomDim2> m_bb;
   };

} // namespace Geometry