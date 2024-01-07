#pragma once

#include "BoundingBox.h"
#include "IGeometryRegion.h"

template<int N>
class IInitialBoundingboxGenerator
{
public:
   ~IInitialBoundingboxGenerator() noexcept = default;
   virtual BoundingBox<double, N> generate(const Geometry::IGeometryRegion<double, N>& region) const = 0;
};