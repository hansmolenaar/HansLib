#pragma once

#include "IUniquePointCollection.h"
#include "IGeometryPredicate.h"
#include "PointExact.h" 

#include <unordered_map>

template<typename T, int N>
class UniqueHashedPointCollection : public IUniquePointCollection<T, N>
{
public:

   int addIfNew(const Point<T, N>&);
   std::optional<PointIndex>  tryGetClosePoint(const Point<T, N>&) const override;
   const IGeometryPredicate<T, N>& getGeometryPredicate() const override;
   Point<T, N> getPoint(PointIndex) const override;
   PointIndex getNumPoints() const override;
private:
   PointExact<T, N > m_predicate;
   std::unordered_map<Point<T, N>, int> m_toIndex;
   std::vector< Point<T, N>> m_points;
};

template<typename T, int N>
const IGeometryPredicate<T, N>& UniqueHashedPointCollection<T, N>::getGeometryPredicate() const
{
   return m_predicate;
}

template<typename T, int N>
std::optional<PointIndex>  UniqueHashedPointCollection<T, N>::tryGetClosePoint(const Point<T, N>& point) const
{
   const auto found = m_toIndex.find(point);
   if (found == m_toIndex.end())
   {
      return {};
   }
   return found->second;
}



template<typename T, int N>
int UniqueHashedPointCollection<T, N>::addIfNew(const Point<T, N>& point)
{
   auto found = m_toIndex.find(point);
   if (found == m_toIndex.end())
   {
      m_toIndex.emplace(point, static_cast<int>(m_points.size()));
      m_points.push_back(point);
      found = m_toIndex.find(point);
   }
   return found->second;
}

template<typename T, int N>
PointIndex UniqueHashedPointCollection<T, N>::getNumPoints() const
{
   return m_points.size();
}

template<typename T, int N>
Point<T, N> UniqueHashedPointCollection<T, N>::getPoint(PointIndex pi) const
{
   return m_points.at(pi);
}