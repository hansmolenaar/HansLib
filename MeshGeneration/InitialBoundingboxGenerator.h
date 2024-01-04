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
   BoundingBox<double, N> generate(typename const BoundingBox<double, N>& bb) const override;

private:
   InitialBoundingboxGenerator(double factor);

   double m_factor;
};
