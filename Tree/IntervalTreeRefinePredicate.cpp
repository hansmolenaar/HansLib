#include "IntervalTreeRefinePredicate.h"

#include "MyException.h"

#include <cmath>

using namespace IntervalTree;
//using namespace Geometry;

template<int N>
bool RefineIfOverlaps<N>::operator()(const Index<N>& indx) const
{
   const BoundingBox<Rational, N> cellBbScaled = indx.getBbOfCell();
   const BoundingBox<double, N> cellBb = BbIntervalTree.scale(cellBbScaled);
   const auto overlap = BoundingBox<double, N>::TryGetOverlap(cellBb, BbGiven);
   return std::get<0>(overlap);
}

template bool  RefineIfOverlaps<1>::operator()(const Index<1>& indx) const;