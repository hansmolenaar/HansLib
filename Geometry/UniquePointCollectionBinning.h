#pragma once

#include "IUniquePointCollection.h"
#include "IGeometryPredicate.h"
#include "LocalizationBins.h"
#include "Defines.h"

template< int N>
class UniquePointCollectionBinning : IUniquePointCollection<double, N>
{
public:
   UniquePointCollectionBinning(const IGeometryPredicate<double, N>& m_predicate, const std::vector<Point<double, N>>& points);

   std::tuple<bool, int>  tryGetClosePoint(const Point<double, N>&) const override;
   const IGeometryPredicate<double, N>& getGeometryPredicate() const override;
   Point<double, N> getPoint(PointIndex) const override;
   PointIndex getNumPoints() const override;

private:
   const IGeometryPredicate<double, N>& m_predicate;
   std::multimap <std::array<size_t, N>, size_t> m_pointInBin;
   std::vector<Point<double, N>> m_points;
   std::vector<LocalizationBins> m_bins;
};

template< int N>
UniquePointCollectionBinning<N>::UniquePointCollectionBinning(const IGeometryPredicate<double, N>& predicate, const std::vector<Point<double, N>>& points) :
   m_predicate(predicate)
{
   for (int n = 0; n < N; ++n)
   {
      std::vector<double> values(points.size());
      str::transform(points, values.begin(), [n](const Point<double, N >& p) {return p[n]; });
      m_bins.emplace_back(LocalizationBins::CreateFromValues(values, false, m_predicate.getSmallLengthInDirection(n)));
   }
}

template< int N>
Point<double, N> UniquePointCollectionBinning<N>::getPoint(PointIndex n) const
{
   return m_points.at(n);
}

template< int N>
PointIndex UniquePointCollectionBinning<N>::getNumPoints() const
{
   return m_points.size();
}

template< int N>
std::tuple<bool, int>  UniquePointCollectionBinning<N>::tryGetClosePoint(const Point<double, N>& p) const
{
   std::array<size_t, N> bins;
   for (int n = 0; n < N; ++n)
   {
      bins[n] = m_bins.at(n).find(p.at(n));
   }
   const auto found = m_pointInBin.find(bins);

   if (found == m_pointInBin.end()) return { false, -1 };
   return { true,static_cast<int>(found->second) };
}


template< int N>
const IGeometryPredicate<double, N>& UniquePointCollectionBinning<N>::getGeometryPredicate() const
{
   return m_predicate;
}
