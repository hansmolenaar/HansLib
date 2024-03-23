#include "Sphere2AsManifold1.h"

using namespace Geometry;

template Sphere2AsManifold1<double>;

template<typename T>
Sphere2AsManifold1<T>::Sphere2AsManifold1(Sphere<T, GeometryDimension> sphere) :
   m_sphere(std::move(sphere))
{
}

template<typename T>
bool Sphere2AsManifold1<T>::contains(const Point<T, 2>& point, const IGeometryPredicate<T, 2>& predicate) const
{
   return m_sphere.Contains(point, predicate);
}


template<typename T>
Line<T, 2> Sphere2AsManifold1<T>::GetEuclideanSubspaceAt(const Point<T, 2>& point) const
{
   throw MyException("Not implemented");
}

template<typename T>
IntersectionDirectedEdges<T, 2> Sphere2AsManifold1<T>::GetIntersectionsWithSimplex(const Simplex<T, 2>& simplex) const
{
   throw MyException("Not implemented");
}