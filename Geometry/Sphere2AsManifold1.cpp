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
Line<T, GeomDim2> Sphere2AsManifold1<T>::getEuclideanSubspaceAt(const Point<T, GeomDim2>& point, const IGeometryPredicate<T, GeomDim2>& predicate) const
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
   std::vector<DirectedEdgePoint<T, GeomDim2>> intersectionPoints;
   const bool firstPointInside = m_sphere.Contains(edge.point0(), predicate);
   const auto firstIntersection = m_sphere.TryGetFirstIntersectionWithDirectedEdge(edge, predicate);

   if (firstPointInside)
   {
      intersectionPoints.emplace_back(edge.point0(), edge, predicate);
      if (firstIntersection)
      {
         intersectionPoints.emplace_back(firstIntersection.value(), edge, predicate);
      }
   }
   else if (firstIntersection.has_value())
   {
      intersectionPoints.emplace_back(firstIntersection.value(), edge, predicate);
      const DirectedEdge<T, GeomDim2> next(firstIntersection.value(), edge.point1());
      if (!next.isDegenerate(predicate))
      {
         const auto secondIntersection = m_sphere.TryGetFirstIntersectionWithDirectedEdge(next, predicate);
         if (secondIntersection.has_value())
         {
            intersectionPoints.emplace_back(secondIntersection.value(), edge, predicate);
         }
      }
   }

   auto containsPoint = [this, &predicate](const Point2& p) {return this->contains(p, predicate); };
   return DirectedEdgeIntersections<T, GeomDim2>(intersectionPoints, containsPoint, predicate);
}

template<typename T>
const std::string& Sphere2AsManifold1<T>::getName() const
{
   return m_name;
}