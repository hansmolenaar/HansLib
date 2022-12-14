#pragma once

#include "Geometry/IUniquePointCollection.h"
#include "Groups/IFinitePointGroupAction.h"
#include "Groups/IFiniteGroupUtils.h"

template<int N>
class FinitePointGroupOrbit : public IUniquePointCollection<double, N>
{
public:
   FinitePointGroupOrbit(const IFinitePointGroupAction<N>&, const Point<double, N>&, const IGeometryPredicate<double, N>&);
   Point<double, N> getPoint(PointIndex) const override;
   PointIndex getNumPoints() const override;
   std::tuple<bool, int>  tryGetClosePoint(const Point<double, N>&) const override;
   const IGeometryPredicate<double, N>& getGeometryPredicate() const override;
private:
   const IGeometryPredicate<double, N>& m_predicate;
   const IFinitePointGroupAction< N>& m_action;
   std::vector<Point<double, N>>  m_uniquePoints;
};

template<int N>
FinitePointGroupOrbit<N>::FinitePointGroupOrbit(const IFinitePointGroupAction<N>& action, const Point<double, N>& point, const IGeometryPredicate<double, N>& predicate) :
   m_predicate(predicate), m_action(action)
{
   for (auto g : IFiniteGroupUtils::GetElements(m_action.getGroup()))
   {
      const auto image = m_action(g, point);
      const auto found = tryGetClosePoint(image);
      if (!std::get<0>(found))
      {
         m_uniquePoints.push_back(image);
      }
   }
}

template<int N>
const IGeometryPredicate<double, N>& FinitePointGroupOrbit<N>::getGeometryPredicate() const
{
   return m_predicate;
}

template<int N>
Point<double, N> FinitePointGroupOrbit<N>::getPoint(PointIndex n) const
{
   return m_uniquePoints.at(n);
}


template<int N>
PointIndex FinitePointGroupOrbit<N>::getNumPoints() const
{
   return static_cast<PointIndex>(m_uniquePoints.size());
}

template<int N>
std::tuple<bool, int>  FinitePointGroupOrbit<N>::tryGetClosePoint(const Point<double, N>& p) const
{
   int result = -1;
   for (const auto& up : m_uniquePoints)
   {
      ++result;
      if (m_predicate.SamePoints(up, p)) return { true,result };
   }
   return { false, -1 };
}