#pragma once

#include "IAnnModel.h"
#include "IAnnCostFunction.h"
#include "IAnnWeights.h"

namespace ML
{
   class AnnModel : public IAnnModel
   {
   public:
      AnnModel(std::span<const ML::IAnnLayer*> layers, std::span<const ML::IAnnWeights*> weightedAverages, const ML::IAnnCostFunction& costFunction);
      std::span<const ML::IAnnLayer*> getLayers() const override;
      std::span<const ML::IAnnWeights*> getWeightedAverages() const override;
      const ML::IAnnCostFunction& getCostFunction() const override;
   private:
      std::span<const ML::IAnnLayer*> m_layers;
      std::span<const ML::IAnnWeights*> m_weightedAverages;
      const ML::IAnnCostFunction& m_costFunction;
   };
}
