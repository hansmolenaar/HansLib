#pragma once

#include "IAnnModel.h"
#include "IAnnCostFunction.h"
#include "IAnnWeightedAverage.h"

namespace ML
{
   class AnnModel : public IAnnModel
   {
   public:
      AnnModel(std::span<const ML::IAnnLayer*> layers, std::span<const ML::IAnnWeightedAverage*> weightedAverages, const ML::IAnnCostFunction& costFunction);
      std::span<const ML::IAnnLayer*> getLayers() const override;
      std::span<const ML::IAnnWeightedAverage*> getWeightedAverages() const override;
      const ML::IAnnCostFunction& getCostFunction() const override;
   private:
      std::span<const ML::IAnnLayer*> m_layers;
      std::span<const ML::IAnnWeightedAverage*> m_weightedAverages;
      const ML::IAnnCostFunction& m_costFunction;
   };
}
