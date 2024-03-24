#include "Ball.h"

using namespace Geometry;

template Ball<int, 1>;
template Ball<int, 2>;
template Ball<int, 3>;
template Ball<double, 1>;
template Ball<double, 2>;
template Ball<double, 3>;


template<typename T, int N>
Ball<T, N>::Ball(Point<T, N> center, T radius) :
   m_center(std::move(center)), m_radius(radius), m_radiusSquared(radius* radius)
{
   Utilities::MyAssert(m_radius > 0);
}

template<typename T, int N>
Point<T, N> Ball<T, N>::getCenter() const
{
   return m_center;
}

template<typename T, int N>
T Ball<T, N>::getRadius() const
{
   return m_radius;
}

template<typename T, int N>
T Ball<T, N>::getRadiusSquared() const
{
   return m_radiusSquared;
}


template<typename T, int N>
BallPosition Ball<T, N>::getPosition(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const
{
   const T distToCenter2 = PointUtils::GetNormSquared(point - m_center);
   if (std::abs(distToCenter2 - m_radiusSquared) <= predicate.getSmallNormSquared()) return On;
   else if (distToCenter2 < m_radiusSquared) return Inside;
   else return Outside;
}

template<typename T, int N>
std::pair<BallPosition, BallPosition> Ball<T, N>::getPositions(const DirectedEdge<T, N>& edge, const IGeometryPredicate<T, N>& predicate) const
{
   return { getPosition(edge.point0(), predicate), getPosition(edge.point1(), predicate) };
}

template<typename T, int N>
BoundingBox<T, N> Ball<T, N>::getBoundingBox() const
{
   std::array<T, N> lwr;
   std::array<T, N> upr;
   str::transform(m_center, lwr.begin(), [this](T cor) {return cor - m_radius; });
   str::transform(m_center, upr.begin(), [this](T cor) {return cor + m_radius; });
   return BoundingBox<T, N>::CreateFromList(std::array<std::array<T, N>, 2> {lwr, upr});
}

template<typename T, int N>
bool Ball<T, N>::Contains(const Point<T, N>& point, const IGeometryPredicate<T, N>& predicate) const
{
   return getPosition(point, predicate) <= On;
}

template<typename T, int N>
std::optional<Point<T, N>> Ball<T, N>::TryGetFirstIntersectionWithDirectedEdge(typename const Geometry::DirectedEdge<T, N>& edge, const IGeometryPredicate<T, N>& predicate) const
{
   const auto [pos0, pos1] = getPositions(edge, predicate);
   if (std::max(pos0, pos1) <= BallPosition::On)
   {
      return  edge.point1();
   }
   const auto& point0 = edge.point0();
   const auto& point1 = edge.point1();
   const auto position0 = getPosition(point0, predicate);
   const auto position1 = getPosition(point1, predicate);

   const T a = edge.lengthSquared();
   T b = 0;
   for (int n = 0; n < N; ++n)
   {
      b += 2 * (edge.point1().at(n) - edge.point0().at(n)) * (edge.point0().at(n) - m_center.at(n));
   }
   T c = PointUtils::GetNormSquared<T, N>(edge.point0() - m_center);
   c -= m_radius * m_radius;

   const T D = b * b - 4 * a * c;
   if (D < 0) return  {};
   const T sqrtD = static_cast<T>(std::sqrt(D));

   const T lam0 = (-b - sqrtD) / (2 * a);
   const auto ip0 = edge.interpolate(lam0);
   Utilities::MyAssert(getPosition(ip0, predicate) == BallPosition::On);

   const T lam1 = (-b + sqrtD) / (2 * a);
   const auto ip1 = edge.interpolate(lam1);
   Utilities::MyAssert(getPosition(ip1, predicate) == BallPosition::On);

   if (pos0 == BallPosition::Inside)
   {
      // Use the second root
      Utilities::MyAssert(pos1 == BallPosition::Outside);
      Utilities::MyAssert(!predicate.SamePoints(point0, ip1));
      return ip1;
   }

   if (pos0 == BallPosition::On)
   {
      Utilities::MyAssert(pos1 == BallPosition::Outside);
      if (edge.contains(ip1, predicate) && !predicate.SamePoints(ip1, point0)) return ip1;
      return {};
   }

   Utilities::MyAssert(pos0 == BallPosition::Outside);
   if (predicate.SamePoints(point1, ip0)) return  point1;
   if (edge.contains(ip0, predicate)) return ip0;
   return {};
}

template<typename T, int N>
bool Ball<T, N>::CouldIntersectWith(typename const BoundingBox<T, N>& bb, const IGeometryPredicate<T, N>& predicate) const
{
   if (!BoundingBox<T, N>::TryGetOverlap(getBoundingBox(), bb)) return false;

   const auto centerBB = bb.getCenter();
   const T diagLength2 = bb.getLengthDiagonalSquared();
   const auto dif = centerBB - getCenter();
   const auto dist2 = PointUtils::GetNormSquared(dif);
   return std::sqrt(dist2) < getRadius() + std::sqrt(diagLength2) + std::sqrt(predicate.getSmallNormSquared());
}