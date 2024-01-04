#include "IntervalTreeRefinePredicate.h"

#include "MyException.h"

#include <cmath>

using namespace IntervalTree;

template<int N>
bool RefineIfOverlaps<N>::operator()(const Index<N>& indx) const
{
   const BoundingBox<Rational, N> cellBbScaled = indx.getBbOfCell();
   const BoundingBox<double, N> cellBb = BbIntervalTree.scaleFrom01(cellBbScaled);
   const auto overlap = BoundingBox<double, N>::TryGetOverlap(cellBb, BbGiven);
   return std::get<0>(overlap);
}

template RefineIfOverlaps<1>;
template RefineIfOverlaps<2>;
template RefineIfOverlaps<3>;