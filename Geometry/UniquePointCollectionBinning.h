#pragma once

#include "IDynamicUniquePointCollection.h"
#include "IGeometryPredicate.h"
#include "LocalizationBins.h"
#include "Defines.h"

#include <unordered_map>

template< int N>
class UniquePointCollectionBinning : public IDynamicUniquePointCollection<double, N>
{
public:
   UniquePointCollectionBinning(const IGeometryPredicate<double, N>& m_predicate, const std::vector<Point<double, N>>& points);

   std::optional<PointIndex>  tryGetClosePoint(const Point<double, N>&) const override;
   const IGeometryPredicate<double, N>& getGeometryPredicate() const override;
   Point<double, N> getPoint(PointIndex) const override;
   PointIndex getNumPoints() const override;

   void deletePoint(PointIndex pointId) override;
   PointIndex addIfNew(const Point<double, N>& point) override;

   const LocalizationBins& getBins(int direction) const;

   std::string toString() const;

   using BinSpecifier = std::array<size_t, N>;
private:

   BinSpecifier locate(const Point<double, N>& point) const;
   std::optional<PointIndex> tryGetClosePointInBin(const Point<double, N>& p, const BinSpecifier& bin) const;

   const IGeometryPredicate<double, N>& m_predicate;
   std::multimap <BinSpecifier, PointIndex> m_pointsInBin;
   std::unordered_map<PointIndex, Point<double, N>> m_points;
   std::vector<LocalizationBins> m_bins;
   PointIndex m_nextPointIndex = 0;
};
