#pragma once

#include "Point.h"
#include "UnitVector.h"
#include "IPointTransformation.h"

template<int N>
class Hyperplane : public IPointTransformation<double, N>
{
public:
   Hyperplane(const Point<double, N>&, std::unique_ptr< UnitVector<N>>&&);
   const UnitVector<N>& getNormal() const { return *m_normal; }
   const Point<double, N>& getReferencePoint() const { return m_referencePoint; }
   double getSignedDistance(Point<double, N>) const;
   Point<double, N> reflect(Point<double, N>) const;

   Point<double, N> operator()(const Point<double, N>& p) const override { return reflect(p); }

private:
   Point<double, N> m_referencePoint;
   std::unique_ptr<UnitVector<N>> m_normal;
};

template<int N>
Hyperplane<N>::Hyperplane(const Point<double, N>& refPont, std::unique_ptr< UnitVector<N>>&& normal) :
   m_referencePoint(refPont), m_normal(std::move(normal))
{
   if (!m_normal) throw MyException("Hyperplane<N>::Hyperplane normal not set");
}

template<int N>
double Hyperplane<N>::getSignedDistance(Point<double, N> point) const
{
   point = point.data() -  m_referencePoint.data();
   return m_normal->innerProduct(point);
}

template<int N>
Point<double, N> Hyperplane<N>::reflect(Point<double, N> point) const
{
   const double dist = getSignedDistance(point);
   const std::array<double, N>& diff = m_normal->data();
   return point.data() - diff * (2 * dist);
}