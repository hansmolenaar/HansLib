#pragma once

#include "IAnnModel.h"

namespace ML
{
   namespace IAnnModelUtils
   {
      void checkDimensions(const ML::IAnnModel& model);
      std::vector<size_t> getLayerDimensions(const ML::IAnnModel& model);
      void updateParameters(const IAnnModel& model, const ML::IParameterSet& parameterDerivs, double learningRate, ML::IParameterSet& parameters);
      void setParameterDerivatives(const IAnnModel& model, const ML::IFeedForwardResult& forwardResult, std::span<const double> ideal, const ML::IParameterSet& parameters, ML::IParameterSet& parameterDerivs);
   };
}
