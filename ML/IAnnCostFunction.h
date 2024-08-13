#pragma once

#include <cstddef>
#include <span>

namespace ML
{
   // Artificial Neural Network, cost-function
   class IAnnCostFunction
   {
   public:
      virtual size_t getNumberOfOutputs() const = 0;
      virtual double calculate(std::span<const double> estimated, std::span<const double> expected) const = 0;
   };
}
