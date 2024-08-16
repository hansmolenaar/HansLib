#pragma once

#include <span>

namespace ML
{
   class IParameterSet
   {
   public:
      virtual ~IParameterSet() = default;
      virtual size_t getNumLayers() const = 0;
      virtual std::span<const double> at(size_t n) const = 0;
      virtual std::span< double> getModifiable(size_t n) = 0;
   };
}
