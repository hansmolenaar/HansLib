#pragma once
#include "IGeometryRegion.h"
#include "IInitialBoundingboxGenerator.h"
#include "IRefinementPredicate.h"
#include "IRefinementPredicateFactory.h"
#include "IntervalTreeIndex.h"

namespace MeshGeneration
{
template <size_t N> class RefineRegionToMaxLevel : public IRefinementPredicate<N>
{
  public:
    RefineRegionToMaxLevel(int maxLevel, const Geometry::IGeometryRegion<double, N> &region,
                           const IGeometryPredicate<double, N> &predicate,
                           const IInitialBoundingboxGenerator<N> &generator);
    bool operator()(const IntervalTree::Index<N> &indx) const override;

  private:
    int m_maxLevel;
    const Geometry::IGeometryRegion<double, N> &m_region;
    BoundingBox<double, N> m_initialBb;
    const IGeometryPredicate<double, N> &m_geometryPredicate;
};

template <size_t N> class RefineRegionToMaxLevelFactory : public IRefinementPredicateFactory<N>
{
  public:
    explicit RefineRegionToMaxLevelFactory<N>(int maxLevel);
    std::unique_ptr<IRefinementPredicate<N>> Create(const Geometry::IGeometryRegion<double, N> &region,
                                                    const IInitialBoundingboxGenerator<N> &generator,
                                                    const IGeometryPredicate<double, N> &geometryPredicate) override;

  private:
    int m_maxLevel;
};

} // namespace MeshGeneration
