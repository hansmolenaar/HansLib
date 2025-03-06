#include "UniquePointCollectionBinning.h"
#include "MultiIndex.h"

#include<boost/container/static_vector.hpp>

template UniquePointCollectionBinning<GeomDim1>;
template UniquePointCollectionBinning<GeomDim2>;
template UniquePointCollectionBinning<GeomDim3>;

using TrialBinsInDir = boost::container::static_vector<size_t, 3>;

namespace
{
   TrialBinsInDir GetCandidateBinsInDirection(double point, const LocalizationBins& binsInDir, double eps)
   {
      TrialBinsInDir result;
      const auto pointInBin = binsInDir.find(point);
      result.push_back(pointInBin);

      if (pointInBin > 0)
      {
         const size_t nxtBinId = pointInBin - 1;
         const double lwrBound = binsInDir.getBinUpper(nxtBinId);
         if (point - lwrBound < eps) result.emplace_back(nxtBinId);
      }
      if (pointInBin + 1 < binsInDir.getNumBins())
      {
         const size_t nxtBinId = pointInBin + 1;
         const double uprBound = binsInDir.getBinLower(nxtBinId);
         if (uprBound - point < eps) result.emplace_back(nxtBinId);
      }
      return result;
   }

}

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
std::optional<PointIndex>  UniquePointCollectionBinning<N>::tryGetClosePoint(const Point<double, N>& p) const
{
   std::array<TrialBinsInDir, N> candidatesInDir;
   for (int n = 0; n < N; ++n)
   {
      candidatesInDir[n] = GetCandidateBinsInDirection(p.at(n), m_bins.at(n), m_predicate.getSmallLengthInDirection(n));
   }

   std::array<size_t, N> dimensions;
   for (int n = 0; n < N; ++n) dimensions.at(n) = candidatesInDir[n].size();
   const auto multiIndex = MultiIndex<size_t>::Create(dimensions);
   const auto numCandidateBins = multiIndex.getFlatSize();
   for (size_t c = 0; c < numCandidateBins; ++c)
   {
      std::array<size_t, N> candidate;
      multiIndex.toMultiplet(c, candidate);
      std::array<size_t, N> singleBin;
      for (int n = 0; n < N; ++n)
      {
         singleBin[n] = candidatesInDir[n].at(candidate.at(n));
      }
      const auto found = tryGetClosePointInBin(p, singleBin);
      if (found) return found;
   }
   return {};
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
UniquePointCollectionBinning<N>::BinSpecifier UniquePointCollectionBinning<N>::locate(const Point<double, N>& point) const
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
   const auto found = tryGetClosePoint(point);
   if (found) return *found;

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
std::optional<PointIndex> UniquePointCollectionBinning<N>::tryGetClosePointInBin(const Point<double, N>& p, const BinSpecifier& bins) const
{
   const auto [first, last] = m_pointsInBin.equal_range(bins);
   for (auto itr = first; itr != last; ++itr)
   {
      const auto pointId = itr->second;
      if (m_predicate.samePoints(p, getPoint(pointId)))
      {
         return static_cast<PointIndex>(pointId);
      }
   }

   return {};
}

template< int N>
std::string UniquePointCollectionBinning<N>::toString() const
{
   const std::string sep = "  ";
   std::ostringstream oss;
   oss << "UniquePointCollectionBinning  NDIR=" << N
      << sep << "NPOINTS=" << getNumPoints() << '\n';
   for (int n = 0; n < N; ++n)
   {
      oss << "DIR=" << n << sep << "->" << sep << m_bins.at(n).toString() << '\n';
   }
   return oss.str();
}

template< int N>
void UniquePointCollectionBinning<N>::movePoint(PointIndex pointId, const Point<double, N>& newCoordinates)
{
   if (!m_points.contains(pointId))
   {
      throw MyException("UniquePointCollectionBinning<N>::movePoint unknown point " + std::to_string(pointId));
   }

   const BinSpecifier location = locate(m_points.at(pointId));
   const auto [first, last] = m_pointsInBin.equal_range(location);
   bool removed = false;
   for (auto itr = first; itr != last; ++itr)
   {
      if (itr->second == pointId)
      {
         m_pointsInBin.erase(itr);
         removed = true;
         break;
      }
   }
   if (!removed)
   {
      throw MyException("UniquePointCollectionBinning<N>::movePoint unable to find pointId in bin");
   }

   const BinSpecifier newLocation = locate(newCoordinates);
   if (tryGetClosePointInBin(newCoordinates, newLocation))
   {
      throw MyException("UniquePointCollectionBinning<N>::movePoint point already exists");
   }
   m_pointsInBin.emplace(newLocation, pointId);
   m_points[pointId] = newCoordinates;
}