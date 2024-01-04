#pragma once

#include "BoundingBox.h"

template<int N>
class IInitialBoundingboxGenerator
{
public:
   ~IInitialBoundingboxGenerator() noexcept = default;
   virtual BoundingBox<double, N> generate(const BoundingBox<double, N>& bb) const = 0;
};