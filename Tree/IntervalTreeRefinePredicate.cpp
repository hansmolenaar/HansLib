#include "IntervalTreeRefinePredicate.h"

#include "MyException.h"

#include <cmath>

using namespace IntervalTree;

template <size_t N> bool RefineIfOverlaps<N>::operator()(const Index<N> &indx) const
{
    const BoundingBox<Rational, N> cellBbScaled = indx.getBbOfCell();
    const BoundingBox<double, N> cellBb = BbIntervalTree.scaleFrom01(cellBbScaled);
    const auto overlap = BoundingBox<double, N>::TryGetOverlap(cellBb, BbGiven);
    return overlap.has_value();
}

template class RefineIfOverlaps<1>;
template class RefineIfOverlaps<2>;
template class RefineIfOverlaps<3>;
