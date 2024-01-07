#include "UniquePointCollectionBinning.h"

template UniquePointCollectionBinning<1>;
template UniquePointCollectionBinning<2>;
template UniquePointCollectionBinning<3>;



template< int N>
UniquePointCollectionBinning<N>::UniquePointCollectionBinning(const IGeometryPredicate<double, N>& predicate, const std::vector<Point<double, N>>& points) :
   m_predicate(predicate)
{
   for (int n = 0; n < N; ++n)
   {
      std::vector<double> values(points.size());
      str::transform(points, values.begin(), [n](const Point<double, N >& p) {return p.data()[n]; });
      m_bins.emplace_back(LocalizationBins::CreateFromValues(values, false, m_predicate.getSmallLengthInDirection(n)));
   }

   for (const auto& point : points)
   {
      addIfNew(point);
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
std::tuple<bool, PointIndex>  UniquePointCollectionBinning<N>::tryGetClosePoint(const Point<double, N>& p) const
{
   std::tuple<bool, PointIndex> result{ false, PointIndexInvalid };
   const auto bins = locate(p);
   auto found = tryGetClosePointInBin(p, bins);
   if (std::get<0>(found)) return found;

   // TODO: Test edge case: point close to bin boundary
   return found;
}


template< int N>
const IGeometryPredicate<double, N>& UniquePointCollectionBinning<N>::getGeometryPredicate() const
{
   return m_predicate;
}

template< int N>
void UniquePointCollectionBinning<N>::deletePoint(PointIndex pointId)
{
   const auto point = getPoint(pointId);
   const auto bins = locate(point);
   const auto [first, last] = m_pointsInBin.equal_range(bins);
   bool found = false;
   for (auto itr = first; itr != last; ++itr)
   {
      if (itr->second == pointId)
      {
         found = true;
         m_pointsInBin.erase(itr);
         break;
      }
   }
   if (!found) throw MyException("UniquePointCollectionBinning<N>::deletePoint() not found: " + std::to_string(pointId));

   m_points.erase(pointId);
}

template< int N>
std::array<size_t, N> UniquePointCollectionBinning<N>::locate(const Point<double, N>& point) const
{
   std::array<size_t, N> bins;
   for (int n = 0; n < N; ++n)
   {
      bins[n] = m_bins.at(n).find(point.at(n));
   }
   return bins;
}

template< int N>
PointIndex UniquePointCollectionBinning<N>::addIfNew(const Point<double, N>& point)
{
   const auto [found, id] = tryGetClosePoint(point);
   if (found) return id;

   const PointIndex result = m_nextPointIndex;
   ++m_nextPointIndex;
   m_points[result] = point;

   const auto bins = locate(point);
   m_pointsInBin.emplace(bins, result);
   return result;
}

template< int N>
const LocalizationBins& UniquePointCollectionBinning<N>::getBins(int direction) const
{
   return m_bins.at(direction);
}

template< int N>
std::tuple<bool, PointIndex> UniquePointCollectionBinning<N>::tryGetClosePointInBin(const Point<double, N>& p, const std::array<size_t, N>& bins) const
{
   std::tuple<bool, PointIndex> result{ false, PointIndexInvalid };
   const auto [first, last] = m_pointsInBin.equal_range(bins);
   for (auto itr = first; itr != last; ++itr)
   {
      const auto pointId = itr->second;
      if (m_predicate.SamePoints(p, getPoint(pointId)))
      {
         return { true,static_cast<PointIndex>(pointId) };
      }
   }

   return result;
}