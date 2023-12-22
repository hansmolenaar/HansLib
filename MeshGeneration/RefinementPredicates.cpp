#include "RefinementPredicates.h"

using namespace MeshGeneration;

template bool MeshGeneration::RefineRegionToMaxLevel<2>::operator()(const IntervalTree::Index<2>& indx) const;

template<int N>
bool MeshGeneration::RefineRegionToMaxLevel<N>::operator()(const IntervalTree::Index<N>&indx) const
{
   if (indx.getLevel() >= MaxLevel) return false;
   return true;
};