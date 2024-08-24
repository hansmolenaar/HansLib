#pragma once

#include <cstddef>
#include <span>

namespace ML
{
   // Artificial Neural Network, layer
   class IAnnLayer
   {
   public:
      virtual ~IAnnLayer() = default;
      virtual size_t getNumberOfNeurons() const = 0;
      virtual void setActivatorFunction(std::span<const double> weightedAverage, std::span<double> activatorValues) const = 0;
      virtual void setActivatorFunctionDeriv(std::span<const double> weightedAverage, std::span<double> activatorDerivValues) const = 0;
   };
}
