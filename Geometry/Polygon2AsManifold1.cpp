#include "DirectedEdge.h"
#include "DirectedEdge.h"
#include "MyException.h"
#include "Polygon2AsManifold1.h"
#include "Polygon2D.h"
#include "UnitVector.h"

using namespace Utilities;
using namespace Geometry;

template Polygon2AsManifold1<double>;

template<typename T>
Polygon2AsManifold1<T>::Polygon2AsManifold1(std::span<const Point<T, 2>> polygon, std::string name) :
   m_polygon(polygon.size()),
   m_name(std::move(name))
{
   str::copy(polygon, m_polygon.begin());
}

template<typename T>
const std::string& Polygon2AsManifold1<T>::getName() const
{
   return m_name;
}

template<typename T>
BoundingBox<T, GeomDim2> Polygon2AsManifold1<T>::getBoundingBox() const
{
   return BoundingBox<T, 2>::CreateFromList(m_polygon);
}

template<typename T>
bool Polygon2AsManifold1<T>::contains(const Point<T, GeomDim2>& point, const IGeometryPredicate<T, GeomDim2>& predicate) const
{
   return Polygon2D::Contains<T>(m_polygon, point, predicate);
}

template<typename T>
Line<T, GeomDim2> Polygon2AsManifold1<T>::GetEuclideanSubspaceAt(const Point<T, GeomDim2>& point, const IGeometryPredicate<T, GeomDim2>& predicate) const
{
   for (size_t n = 0; n < m_polygon.size(); ++n)
   {
      const Point<T, GeomDim2> p0 = m_polygon.at(n);
      const Point<T, GeomDim2> p1 = m_polygon.at((n + 1) % m_polygon.size());
      const DirectedEdge<T, GeomDim2> edge(p0, p1);
      if (edge.contains(point, predicate))
      {
         return Line<T, GeomDim2>(p0, *UnitVector<T, GeomDim2>::Create(p0, p1));
      }
   }
   throw MyException("Polygon2AsManifold1<T>::GetEuclideanSubspaceAt polygon does not contain point");
}

template<typename T>
DirectedEdgeIntersections<T, GeomDim2> Polygon2AsManifold1<T>::GetIntersections(const DirectedEdge<T, GeomDim2>& edge, const IGeometryPredicate<T, GeomDim2>& predicate) const
{
   throw MyException("not yet implemented");
}