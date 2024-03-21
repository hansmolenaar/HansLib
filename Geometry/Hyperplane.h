#pragma once

#include "Point.h"
#include "UnitVector.h"
#include "IPointTransformation.h"

template<typename T, int N>
class Hyperplane : public IPointTransformation<T, N>
{
public:
   Hyperplane(const Point<T, N>&, std::unique_ptr< UnitVector<T,N>>&&);
   const UnitVector<T,N>& getNormal() const { return *m_normal; }
   const Point<T, N>& getReferencePoint() const { return m_referencePoint; }
   T getSignedDistance(Point<T, N>) const;
   Point<T, N> reflect(Point<T, N>) const;

   Point<T, N> operator()(const Point<T, N>& p) const override { return reflect(p); }

private:
   Point<T, N> m_referencePoint;
   std::unique_ptr<UnitVector<T,N>> m_normal;
};

template<typename T, int N>
Hyperplane<T, N>::Hyperplane(const Point<T, N>& refPont, std::unique_ptr< UnitVector<T,N>>&& normal) :
   m_referencePoint(refPont), m_normal(std::move(normal))
{
   if (!m_normal) throw MyException("Hyperplane<N>::Hyperplane normal not set");
}

template<typename T, int N>
T Hyperplane<T, N>::getSignedDistance(Point<T, N> point) const
{
   return m_normal->innerProduct(point - m_referencePoint);
}

template<typename T, int N>
Point<T, N> Hyperplane<T, N>::reflect(Point<T, N> point) const
{
   const T dist = getSignedDistance(point);
   return point - m_normal->data() * (2 * dist);
}