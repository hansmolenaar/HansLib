#include "Ball2AsRegion.h"
#include "MyException.h"

using namespace Geometry;

template Ball2AsRegion<double>;

template<typename T>
Ball2AsRegion<T>::Ball2AsRegion(Ball<T, 2> ball) : m_ball(std::move(ball))
{
}

template<typename T>
BoundingBox<T, 2> Ball2AsRegion<T>::getBoundingBox() const
{
   return m_ball.getBoundingBox();
}

template<typename T>
bool Ball2AsRegion<T>::Contains(const Point<T, GeomDim>& point, const IGeometryPredicate<T, GeomDim>& predicate) const
{
   return m_ball.Contains(point, predicate);
}

template<typename T>
bool Ball2AsRegion<T>::CouldIntersectWith( const BoundingBox<T, GeomDim>& bb, const IGeometryPredicate<T, GeomDim>& predicate) const
{
   return m_ball.CouldIntersectWith(bb, predicate);
}

template<typename T>
const IRegionManifolds<T, 2>& Ball2AsRegion<T>::getManifolds() const
{
   throw MyException("Not yet implemented");
}