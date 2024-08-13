#pragma once

#include "IAnnLayer.h"
#include "IAnnCostFunction.h"
#include "IAnnWeightedAverage.h"

namespace ML
{
   // Artificial Neural Network, full model
   class IAnnModel
   {
   public:
      virtual size_t getNumberOfLayers() const = 0;
      std::span<const ML::IAnnLayer*> getLayers() const = 0;
      std::span<const ML::IAnnWeightedAverage*> getWeightedAverates() const = 0;
      const ML::IAnnCostFunction& getCostFunction() const = 0;
      virtual size_t getBatchSize() const = 0;
      virtual std::span<const double> getTrainingData(size_t n) const = 0;
      virtual std::span<const double> getInitialParameters(size_t layer) const = 0;
   };
}
