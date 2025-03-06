#include "DirectedEdge2AsManifold1.h"
#include "DirectedEdgeUtils.h"

using namespace Geometry;

template DirectedEdge2AsManifold1<double>;

template<typename T>
DirectedEdge2AsManifold1<T>::DirectedEdge2AsManifold1(DirectedEdge<T, GeomDim2> edge, std::string name) :
   m_name(std::move(name)),
   m_edge(std::move(edge)),
   m_line(m_edge.point0(), m_edge.point1()),
   m_bb(m_edge.getBoundingBox())
{
}

template<typename T>
bool DirectedEdge2AsManifold1<T>::contains(const Point<T, GeomDim2>& point, const IGeometryPredicate<T, GeomDim2>& predicate) const
{
   return m_edge.contains(point, predicate);
}


template<typename T>
Line<T, GeomDim2> DirectedEdge2AsManifold1<T>::getEuclideanSubspaceAt(const Point<T, GeomDim2>& point, const IGeometryPredicate<T, GeomDim2>& predicate) const
{
   if (!contains(point, predicate))
   {
      throw MyException("DirectedEdge2AsManifold1<T>::GetEuclideanSubspaceAt, point not on manifold");
   }
   return m_line;
}

template<typename T>
BoundingBox<T, GeomDim2> DirectedEdge2AsManifold1<T>::getBoundingBox() const
{
   return m_bb;
}

template<typename T>
DirectedEdgeIntersections<T, GeomDim2> DirectedEdge2AsManifold1<T>::GetIntersections(const DirectedEdge<T, GeomDim2>& edge, const IGeometryPredicate<T, GeomDim2>& predicate) const
{
   auto intersection = DirectedEdgeUtils::intersectWith2(edge, m_edge, predicate);
   if (!intersection) return {};
   return { *intersection };
}

template<typename T>
const std::string& DirectedEdge2AsManifold1<T>::getName() const
{
   return m_name;
}