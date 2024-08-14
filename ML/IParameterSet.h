#pragma once

#include <span>

namespace ML
{
   class IParameterSet
   {
   public:
      virtual std::span<const double> at(size_t n) const = 0;
   };
}
