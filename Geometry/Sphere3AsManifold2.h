#pragma once

#include "Defines.h"
#include "IManifold2.h"
#include "Sphere.h"

namespace Geometry
{
template <typename T> class Sphere3AsManifold2 : public IManifold2<T>
{

  public:
    explicit Sphere3AsManifold2(Sphere<T, GeomDim3> sphere, std::string name = "Sphere3AsManifold2");

    bool contains(const Point<T, GeomDim3> &point, const IGeometryPredicate<T, GeomDim3> &predicate) const override;

    Hyperplane<T, GeomDim3> getEuclideanSubspaceAt(const Point<T, GeomDim3> &point) const override;
    DirectedEdgeIntersections<T, GeomDim3> getIntersections(
        const DirectedEdge<T, GeomDim3> &edge, const IGeometryPredicate<T, GeomDim3> &predicate) const override;

    BoundingBox<T, GeomDim3> getBoundingBox() const override;

    const std::string &getName() const override;

  private:
    Sphere<T, GeomDim3> m_sphere;
    std::string m_name;
};

} // namespace Geometry