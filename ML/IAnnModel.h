#pragma once

#include "IAnnLayer.h"
#include "IAnnCostFunction.h"
#include "IAnnWeightedAverage.h"
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
      virtual std::span<const ML::IAnnWeightedAverage*> getWeightedAverages() const = 0;
      virtual const ML::IAnnCostFunction& getCostFunction() const = 0;

      // Implemented functions
      void checkDimensions() const;
      std::vector<size_t> getLayerDimensions() const;
      std::unique_ptr<ML::IFeedForwardResult> feedForward(std::span<const double> input, const ML::IParameterSet& parameterSet) const;
      double calculateError(const ML::IAnnDataSet& dataSet, const ML::IParameterSet& parameterSet) const;
      void backPropagation( const ML::IFeedForwardResult& forwardResult, std::span<const double> ideal, double learningRate, ML::IParameterSet& parameterSet) const;
   };
}
