#pragma once

#include "IAnnCostFunction.h"
#include "IAnnModel.h"
#include "IAnnWeights.h"

namespace ML
{
class AnnModel : public IAnnModel
{
  public:
    AnnModel(std::span<const ML::IAnnLayer *> layers, std::span<const ML::IAnnWeights *> weightedAverages,
             const ML::IAnnCostFunction &costFunction);
    std::span<const ML::IAnnLayer *> getLayers() const override;
    std::span<const ML::IAnnWeights *> getWeights() const override;
    const ML::IAnnCostFunction &getCostFunction() const override;

  private:
    std::span<const ML::IAnnLayer *> m_layers;
    std::span<const ML::IAnnWeights *> m_weights;
    const ML::IAnnCostFunction &m_costFunction;
};
} // namespace ML
