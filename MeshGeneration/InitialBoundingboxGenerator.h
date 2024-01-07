#pragma once

#include "IInitialBoundingboxGenerator.h"
#include "BoundingBox.h"
#include <memory>

template<int N>
class InitialBoundingboxGenerator : public IInitialBoundingboxGenerator<N>
{
public:
   InitialBoundingboxGenerator(InitialBoundingboxGenerator&& other) = default;
   InitialBoundingboxGenerator& operator=(InitialBoundingboxGenerator&& other) = default;

   static std::unique_ptr<IInitialBoundingboxGenerator<N>> Create(double factor);
   BoundingBox<double, N> generate(const Geometry::IGeometryRegion<double, N>& region) const override;

private:
   InitialBoundingboxGenerator(double factor);

   double m_factor;
};
