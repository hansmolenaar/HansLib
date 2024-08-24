#pragma once

#include "IAnnModel.h"
#include "AnnArray.h"

namespace ML
{
   namespace IAnnModelUtils
   {
      void checkDimensions(const ML::IAnnModel& model);
      std::vector<size_t> getLayerDimensions(const ML::IAnnModel& model);
      void updateParameters(const IAnnModel& model, const ML::IParameterSet& parameterDerivs, double learningRate, ML::IParameterSet& parameters);
      void setParameterDerivatives(const IAnnModel& model, const ML::IFeedForwardResult& forwardResult, std::span<const double> ideal, const ML::IParameterSet& parameters, ML::IParameterSet& parameterDerivs);
      void setErrors(const IAnnModel& model, const ML::IFeedForwardResult& forwardResult, std::span<const double> ideal, const ML::IParameterSet& parameters, ML::AnnArray& neuronError);
      double getCost(const IAnnModel& model, const ML::IAnnDataSet& dataSet, const ML::IParameterSet& parameterSet);
      std::unique_ptr<ML::IFeedForwardResult> feedForward(const IAnnModel& model, std::span<const double> input, const ML::IParameterSet& parameterSet);
      void backPropagation(const IAnnModel& model, const ML::IFeedForwardResult& forwardResult, std::span<const double> ideal, double learningRate, ML::IParameterSet& parameterSet);
   };
}
