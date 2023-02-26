#pragma once

#include "Point.h"

#include <limits>

using KdTreePosition = int;

constexpr KdTreePosition KdTreePositionInvalid = std::numeric_limits<KdTreePosition>::min();

enum class KdTreeOverlap { NoOverlap, Overlap, Contains };

template<typename T, int N>
class IKdTreeTraversor
{
public:
   virtual ~IKdTreeTraversor() = default;
   virtual void HandleLeaf(KdTreePosition, const Point<T,N>&) = 0;
   virtual KdTreeOverlap DeterminOverlap(const Point<T, N>&, const Point<T, N>&) const = 0;
};
