#pragma once

#include "IAnnLayer.h"
#include "IAnnCostFunction.h"
#include "IAnnWeightedAverage.h"
#include "IFeedForwardResult.h"
#include "IParameterSet.h"
#include <memory>

namespace ML
{
   // Artificial Neural Network, full model
   class IAnnModel
   {
   public:
      virtual std::span<const ML::IAnnLayer*> getLayers() const = 0;
      virtual std::span<const ML::IAnnWeightedAverage*> getWeightedAverages() const = 0;
      virtual const ML::IAnnCostFunction& getCostFunction() const = 0;

      void checkDimensions() const;
      std::unique_ptr<ML::IFeedForwardResult> feedForward(std::span<const double> input, const ML::IParameterSet& parameterSet) const;
   };
}
