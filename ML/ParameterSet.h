#pragma once

#include "IParameterSet.h"
#include <initializer_list>
#include <vector>

namespace ML
{
   class ParameterSet : public IParameterSet
   {
   public:
      static ParameterSet CreateUsingDimensions(const ML::IParameterSet& parameterSet);
      void add(std::initializer_list<double> parametersForTransformation);
      void add(std::vector<double> parametersForTransformation);

      size_t getNumLayers() const override;
      std::span<const double> at(size_t n) const override;
      std::span< double> getModifiable(size_t n) override;
   private:
      std::vector<std::vector<double>> m_allParameters;
   };
}
