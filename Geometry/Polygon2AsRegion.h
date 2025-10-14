#pragma once

#include "Defines.h"
#include "DirectedEdge2AsManifold1.h"
#include "IGeometryRegion.h"
#include "IRegionManifolds.h"
#include "Manifold0.h"

namespace Geometry
{
   template<typename T>
   class Polygon2AsRegion : public IGeometryRegion<T, GeomDim2>, public IRegionManifolds<T, GeomDim2>
   {

   public:
      Polygon2AsRegion(std::span<const Point<T, 2>> polygon, std::string name);

      bool contains(const Point<T, GeomDim2>& point, const IGeometryPredicate<T, GeomDim2>& predicate) const override;

      const IRegionManifolds<T, GeomDim2>& getManifolds() const override;

      BoundingBox<T, GeomDim2> getBoundingBox() const override;

      const std::string& getName() const override;

      std::vector<const IManifold<T, GeomDim2>*> getAllManifolds() const override;

      // Ignore lower dimensional manifolds
      std::vector<const IManifold<T, GeomDim2>*> getBoundaryHyperManifolds() const override;

      // Adjacencies
      std::vector<const IManifold<T, GeomDim2>*> getConnectedLowers(const IManifold<T, GeomDim2>& manifold) const override;
      std::vector<const IManifold<T, GeomDim2>*> getConnectedHighers(const IManifold<T, GeomDim2>& manifold) const override;

   private:
      std::vector <Point<T, GeomDim2>>  m_polygon;
      BoundingBox<T, GeomDim2> m_bb;
      std::string m_name;
      std::vector<Manifold0<T, GeomDim2>> m_pointManifolds;
      std::vector< DirectedEdge2AsManifold1<T>> m_edgeManifolds;
   };

} // namespace Geometry