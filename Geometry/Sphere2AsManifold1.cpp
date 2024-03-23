#include "Sphere2AsManifold1.h"

using namespace Geometry;

template Sphere2AsManifold1<double>;

template<typename T>
Sphere2AsManifold1<T>::Sphere2AsManifold1(Sphere<T, GeomDim> sphere) :
   m_sphere(std::move(sphere))
{
}

template<typename T>
bool Sphere2AsManifold1<T>::contains(const Point<T, GeomDim>& point, const IGeometryPredicate<T, GeomDim>& predicate) const
{
   return m_sphere.Contains(point, predicate);
}


template<typename T>
Line<T, Sphere2AsManifold1<T>::GeomDim> Sphere2AsManifold1<T>::GetEuclideanSubspaceAt(const Point<T, GeomDim>& point, const IGeometryPredicate<T, GeomDim>& predicate) const
{
   if (!contains(point, predicate))
   {
      throw MyException("Sphere2AsManifold1<T>::GetEuclideanSubspaceAt, point not on manifold");
   }
   const auto radial = point - m_sphere.getCenter();
   const auto uv = UnitVector<T, 2>::Create({ -radial[1], radial[0] }).value();
   return { point, uv };
}

template<typename T>
DirectedEdgeIntersections<T, 2> Sphere2AsManifold1<T>::GetIntersections(const DirectedEdge<T, GeomDim>& edge, const IGeometryPredicate<T, GeomDim>& predicate) const
{
   const bool firstPointInside = m_sphere.Contains(edge.point0(), predicate);
   const auto firstIntersection = m_sphere.TryGetFirstIntersectionWithDirectedEdge(edge, predicate);
   DirectedEdgeIntersections<T, GeomDim> result;
   if (firstPointInside)
   {
      result.emplace_back(edge.point0());
      if (firstIntersection)
      {
         result.emplace_back(firstIntersection.value());
      }
   }
   else if (firstIntersection)
   {
      result.emplace_back(firstIntersection.value());
      const DirectedEdge<T, GeomDim> next  = DirectedEdge<T, GeomDim>::Create(firstIntersection.value(), edge.point1());
      if (!next.isDegenerate(predicate))
      {
         const auto secondIntersection = m_sphere.TryGetFirstIntersectionWithDirectedEdge(next, predicate);
         result.emplace_back(secondIntersection.value());
      }
   }
   return result;
}