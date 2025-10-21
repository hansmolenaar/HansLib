#include "Sphere3AsManifold2.h"

using namespace Geometry;

template class Sphere3AsManifold2<double>;

template <typename T>
Sphere3AsManifold2<T>::Sphere3AsManifold2(Sphere<T, GeomDim3> sphere, std::string name)
    : m_sphere(std::move(sphere)), m_name(std::move(name))
{
}

template <typename T>
bool Sphere3AsManifold2<T>::contains(const Point<T, GeomDim3> &point,
                                     const IGeometryPredicate<T, GeomDim3> &predicate) const
{
    return m_sphere.Contains(point, predicate);
}

template <typename T>
Hyperplane<T, GeomDim3> Sphere3AsManifold2<T>::getEuclideanSubspaceAt(const Point<T, GeomDim3> &point) const
{
    throw MyException("Sphere3AsManifold2<T>::GetEuclideanSubspaceAt, not yet implemented");
}

template <typename T> BoundingBox<T, GeomDim3> Sphere3AsManifold2<T>::getBoundingBox() const
{
    return m_sphere.getBoundingBox();
}

template <typename T>
DirectedEdgeIntersections<T, GeomDim3> Sphere3AsManifold2<T>::getIntersections(
    const DirectedEdge<T, GeomDim3> &edge, const IGeometryPredicate<T, GeomDim3> &predicate) const
{
    return m_sphere.getIntersections(edge, predicate);
}

template <typename T> const std::string &Sphere3AsManifold2<T>::getName() const
{
    return m_name;
}
