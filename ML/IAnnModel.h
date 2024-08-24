#pragma once

#include "IAnnLayer.h"
#include "IAnnCostFunction.h"
#include "IAnnWeights.h"
#include "IFeedForwardResult.h"
#include "IParameterSet.h"

#include <vector>
#include <memory>

namespace ML
{
   // Artificial Neural Network, full model
   class IAnnModel
   {
   public:
      virtual ~IAnnModel() = default;
      virtual std::span<const ML::IAnnLayer*> getLayers() const = 0;
      virtual std::span<const ML::IAnnWeights*> getWeightedAverages() const = 0;
      virtual const ML::IAnnCostFunction& getCostFunction() const = 0;
   };
}
