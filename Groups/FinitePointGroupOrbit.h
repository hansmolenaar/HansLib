#pragma once

#include "Geometry/IUniquePointCollection.h"
#include "Groups/IFinitePointGroupAction.h"
#include "Geometry/IGeometryPredicate.h"

template<int N>
class FinitePointGroupOrbit : public IUniquePointCollection<double, N>
{
public:
   FinitePointGroupOrbit(const IFinitePointGroupAction<N>&, const Point<double, N>&, const IGeometryPredicate<double, N>&);
   Point<double, N> getPoint(int) override;
   std::tuple<bool, int>  tryGetClosePoint(const Point<double, N>&) const override;
   const IGeometryPredicate<double,N>& getGeometryPredicate() const override;
private:
   const IGeometryPredicate<double, N>& m_predicate;
   const IFinitePointGroupAction< N>& m_action;
   std::vector<Point<double, N>>  m_uniquePoints;
   std::vector<int> m_toUniquePoint;
};

template<int N>
FinitePointGroupOrbit<N>::FinitePointGroupOrbit(const IFinitePointGroupAction<N>& action, const Point<double, N>& point, const IGeometryPredicate<double, N>& predicate) : 
   m_predicate(predicate), m_action(action)
{

}

template<int N>
const IGeometryPredicate<double, N>& FinitePointGroupOrbit<N>::getGeometryPredicate() const
{
   return m_predicate;
}