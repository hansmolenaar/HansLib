#include "Sphere.h"

using namespace Geometry;

template Sphere<double, 1>;
template Sphere<double, 2>;
template Sphere<double, 3>;


template<typename T, int N>
Sphere<T, N>::Sphere(Point<T, N> center, T radius) :
   m_ball(std::move(center), radius)
{
}


template<typename T, int N>
BoundingBox<T, N> Sphere<T, N>::getBoundingBox() const
{
   return m_ball.getBoundingBox();
}

template<typename T, int N>
bool Sphere<T, N>::Contains(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const
{
   return m_ball.getPosition(point, predicate) == BallPosition::On;
}

template<typename T, int N>
std::optional<Point<T, N>> Sphere<T, N>::TryGetFirstIntersectionWithDirectedEdge(typename const Geometry::DirectedEdge<T, N>& edge, const IGeometryPredicate<T, N>& predicate) const
{
   const auto& point0 = edge.point0();
   const auto& point1 = edge.point1();

   const auto [pos0, pos1] = m_ball.getPositions(edge, predicate);

   if (pos0 == BallPosition::Inside && pos1 == BallPosition::Inside) return {};
   if (pos0 == BallPosition::Inside && pos1 == BallPosition::On) return  point1;
   if (pos0 == BallPosition::On && pos1 == BallPosition::Inside) return  {};
   if (pos0 == BallPosition::On && pos1 == BallPosition::On) return  point1;

   const auto ip = m_ball.TryGetFirstIntersectionWithDirectedEdge(edge, predicate);

   if (pos0 == BallPosition::Inside)
   {
      Utilities::MyAssert(pos1 == BallPosition::Outside);
      return *ip;
   }

   if (pos0 == BallPosition::On)
   {
      Utilities::MyAssert(pos1 == BallPosition::Outside);
      return ip;
   }

   Utilities::MyAssert(pos0 == BallPosition::Outside);
   return  ip;
}

template<typename T, int N>
bool Sphere<T, N>::CouldIntersectWith(typename const BoundingBox<T, N>& bb, const IGeometryPredicate<T, N>& predicate) const
{
   if (!BoundingBox<T, N>::TryGetOverlap(getBoundingBox(), bb)) return false;

   // If the whole bb is inside the sphere, there is no intersection
   if (m_ball.getPosition(bb.getLower(), predicate) != BallPosition::Inside) return true;
   if (m_ball.getPosition(bb.getUpper(), predicate) != BallPosition::Inside) return true;
   return false;
}

template<typename T, int N>
Point<T, N> Sphere<T, N>::getCenter() const
{
   return m_ball.getCenter();
}

