#include "Sphere2AsManifold1.h"

using namespace Geometry;

template Sphere2AsManifold1<double>;

template<typename T>
Sphere2AsManifold1<T>::Sphere2AsManifold1(Sphere<T, GeomDim2> sphere, std::string name) :
   m_sphere(std::move(sphere)),
   m_name(std::move(name))
{
}

template<typename T>
bool Sphere2AsManifold1<T>::contains(const Point<T, GeomDim2>& point, const IGeometryPredicate<T, GeomDim2>& predicate) const
{
   return m_sphere.Contains(point, predicate);
}


template<typename T>
Line<T, GeomDim2> Sphere2AsManifold1<T>::GetEuclideanSubspaceAt(const Point<T, GeomDim2>& point, const IGeometryPredicate<T, GeomDim2>& predicate) const
{
   if (!contains(point, predicate))
   {
      throw MyException("Sphere2AsManifold1<T>::GetEuclideanSubspaceAt, point not on manifold");
   }
   const auto radial = point - m_sphere.getCenter();
   const auto uv = UnitVector<T, GeomDim2>::Create({ -radial[1], radial[0] }).value();
   return { point, uv };
}

template<typename T>
BoundingBox<T, GeomDim2> Sphere2AsManifold1<T>::getBoundingBox() const
{
   return m_sphere.getBoundingBox();
}

template<typename T>
DirectedEdgeIntersections<T, GeomDim2> Sphere2AsManifold1<T>::GetIntersections(const DirectedEdge<T, GeomDim2>& edge, const IGeometryPredicate<T, GeomDim2>& predicate) const
{
   if (edge.isDegenerate(predicate))
   {
      throw MyException("Sphere2AsManifold1<T>::GetIntersections degenerate edge");
   }
   const bool firstPointInside = m_sphere.Contains(edge.point0(), predicate);
   const auto firstIntersection = m_sphere.TryGetFirstIntersectionWithDirectedEdge(edge, predicate);
   DirectedEdgeIntersections<T, GeomDim2> result;
   if (firstPointInside)
   {
      result.emplace_back(edge.createEdgePoint(edge.point0(), predicate));
      if (firstIntersection)
      {
         result.emplace_back(edge.createEdgePoint(firstIntersection.value(), predicate));
      }
   }
   else if (firstIntersection.has_value())
   {
      result.emplace_back(edge.createEdgePoint(firstIntersection.value(), predicate));
      const DirectedEdge<T, GeomDim2> next(firstIntersection.value(), edge.point1());
      if (!next.isDegenerate(predicate))
      {
         const auto secondIntersection = m_sphere.TryGetFirstIntersectionWithDirectedEdge(next, predicate);
         if (secondIntersection.has_value())
         {
            if (!predicate.SamePoints(firstIntersection.value(), secondIntersection.value()))
            {
               result.emplace_back(edge.createEdgePoint(secondIntersection.value(), predicate));
            }
         }
      }
   }
   return result;
}

template<typename T>
const std::string& Sphere2AsManifold1<T>::getName() const
{
   return m_name;
}