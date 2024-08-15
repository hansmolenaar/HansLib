#pragma once

#include "IParameterSet.h"
#include <initializer_list>
#include <vector>

namespace ML
{
   class ParameterSet : public IParameterSet
   {
   public:
      void add(std::initializer_list<double> parametersForTransformation);
      void add(std::vector<double> parametersForTransformation);

      size_t getNumLayers() const override;
      std::span<const double> at(size_t n) const override;
   private:
      std::vector<std::vector<double>> m_allParameters;
   };
}
