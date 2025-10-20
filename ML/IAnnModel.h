#pragma once

#include "IAnnCostFunction.h"
#include "IAnnLayer.h"
#include "IAnnWeights.h"
#include "IFeedForwardResult.h"
#include "IParameterSet.h"

#include <memory>
#include <vector>

namespace ML
{
// Artificial Neural Network, full model
class IAnnModel
{
  public:
    virtual ~IAnnModel() = default;
    virtual std::span<const ML::IAnnLayer *> getLayers() const = 0;
    virtual std::span<const ML::IAnnWeights *> getWeights() const = 0;
    virtual const ML::IAnnCostFunction &getCostFunction() const = 0;
};
} // namespace ML
