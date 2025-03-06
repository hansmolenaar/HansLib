#include "DirectedEdge.h"
#include "DirectedEdge.h"
#include "MyException.h"
#include "Polygon2AsRegion.h"
#include "Polygon2D.h"
#include "UnitVector.h"

using namespace Utilities;
using namespace Geometry;

template Polygon2AsRegion<double>;

template<typename T>
Polygon2AsRegion<T>::Polygon2AsRegion(std::span<const Point<T, 2>> polygon, std::string name) :
   m_polygon(polygon.size()),
   m_name(std::move(name))
{
   str::copy(polygon, m_polygon.begin());
}

template<typename T>
const std::string& Polygon2AsRegion<T>::getName() const
{
   return m_name;
}

template<typename T>
BoundingBox<T, GeomDim2> Polygon2AsRegion<T>::getBoundingBox() const
{
   return BoundingBox<T, 2>::CreateFromList(m_polygon);
}

template<typename T>
bool Polygon2AsRegion<T>::contains(const Point<T, GeomDim2>& point, const IGeometryPredicate<T, GeomDim2>& predicate) const
{
   return Polygon2D::Contains<T>(m_polygon, point, predicate);
}

template<typename T>
bool Polygon2AsRegion<T>::couldIntersectWith(typename const BoundingBox<T, GeomDim2>& bb, const IGeometryPredicate<T, GeomDim2>& predicate) const
{
   throw MyException("Polygon2AsRegion<T>::CouldIntersectWith() not yet implemented");
}

template<typename T>
const IRegionManifolds<T, GeomDim2>& Polygon2AsRegion<T>::getManifolds() const
{
   throw MyException("Polygon2AsRegion<T>::getManifolds() not yet implemented");
}