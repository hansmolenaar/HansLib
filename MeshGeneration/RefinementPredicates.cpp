#include "RefinementPredicates.h"
#include "IGeometryPredicate.h"

using namespace MeshGeneration;

template RefineRegionToMaxLevel<2>;
template RefineRegionToMaxLevel<3>;

template<int N>
RefineRegionToMaxLevel<N>::RefineRegionToMaxLevel(int maxLevel, const Geometry::IGeometryRegion<double, N>& region, const IGeometryPredicate<double, N>& predicate,
   const IInitialBoundingboxGenerator<N>& generator) :
   m_maxLevel(maxLevel), m_region(region), m_initialBb(generator.generate(m_region.getBoundingBox())), m_geometryPredicate(predicate)
{}

template<int N>
bool MeshGeneration::RefineRegionToMaxLevel<N>::operator()(const IntervalTree::Index<N>& indx) const
{
   if (indx.getLevel() >= m_maxLevel) return false;
   const auto& indxBb = m_initialBb.scaleFrom01(indx.getBbOfCell());
   return m_region.CouldIntersectWith(indxBb, m_geometryPredicate);
};