#include "RefinementPredicates.h"

using namespace MeshGeneration;

template RefineRegionToMaxLevel<2>;

template<int N>
RefineRegionToMaxLevel<N>::RefineRegionToMaxLevel(int maxLevel, const Geometry::IGeometryRegion<double, N>& region) :
   m_maxLevel(maxLevel), m_region(region)
{}

template<int N>
bool MeshGeneration::RefineRegionToMaxLevel<N>::operator()(const IntervalTree::Index<N>&indx) const
{
   if (indx.getLevel() >= m_maxLevel) return false;
   return true;
};