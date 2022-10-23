#pragma once

#include "Point/Point.h"

using KdTreePosition = int;

enum class KdTreeOverlap { NoOverlap, Overlap, Contains };

template<typename T, int N>
class IKdTreeTraversor
{
public:
   virtual ~IKdTreeTraversor() = default;
   virtual void HandleLeaf(KdTreePosition) = 0;
   virtual KdTreeOverlap DeterminOverlap(const Point<T, N>&, const Point<T, N>&) const = 0;
};
