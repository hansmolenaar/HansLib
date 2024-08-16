#pragma once

#include <cstddef>
#include <span>

namespace ML
{
   // Artificial Neural Network, affine transformation between layers
   class IAnnWeightedAverage
   {
   public:
      virtual ~IAnnWeightedAverage() = default;
      virtual size_t getNumberOfNeuronsPrv() const = 0;
      virtual size_t getNumberOfNeuronsCur() const = 0;
      virtual size_t getNumberOfParameters() const = 0;
      virtual void transform(std::span<const double> activatorValuesPrv, std::span<const double> params, std::span<double> weightedAverage) const = 0;
      virtual void backpropInit(std::span<const double> activatorValuesPrv, std::span<const double> dError_dWeightedAverageLast, std::span<double> dError_dParam) const = 0;
   };
}
