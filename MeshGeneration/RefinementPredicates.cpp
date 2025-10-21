#include "RefinementPredicates.h"
#include "IGeometryPredicate.h"

using namespace MeshGeneration;

template class RefineRegionToMaxLevel<2>;
template class RefineRegionToMaxLevel<3>;
template class RefineRegionToMaxLevelFactory<2>;
template class RefineRegionToMaxLevelFactory<3>;

template <size_t N>
RefineRegionToMaxLevel<N>::RefineRegionToMaxLevel(int maxLevel, const Geometry::IGeometryRegion<double, N> &region,
                                                  const IGeometryPredicate<double, N> &predicate,
                                                  const IInitialBoundingboxGenerator<N> &generator)
    : m_maxLevel(maxLevel), m_region(region), m_initialBb(generator.generate(m_region)), m_geometryPredicate(predicate)
{
}

template <size_t N> bool MeshGeneration::RefineRegionToMaxLevel<N>::operator()(const IntervalTree::Index<N> &indx) const
{
    if (indx.getLevel() >= m_maxLevel)
        return false;
    const auto &indxBb = m_initialBb.scaleFrom01(indx.getBbOfCell());
    return m_region.couldIntersectWith(indxBb, m_geometryPredicate);
};

template <size_t N> RefineRegionToMaxLevelFactory<N>::RefineRegionToMaxLevelFactory(int maxLevel) : m_maxLevel(maxLevel)
{
}

template <size_t N>
std::unique_ptr<IRefinementPredicate<N>> RefineRegionToMaxLevelFactory<N>::Create(
    const Geometry::IGeometryRegion<double, N> &region, const IInitialBoundingboxGenerator<N> &generator,
    const IGeometryPredicate<double, N> &geometryPredicate)
{
    return std::make_unique<RefineRegionToMaxLevel<N>>(m_maxLevel, region, geometryPredicate, generator);
}
