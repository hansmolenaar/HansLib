#pragma once

#include <cstddef>
#include <span>

namespace ML
{
   // Artificial Neural Network, affine transformation between layers
   class IAnnWeights
   {
   public:
      virtual ~IAnnWeights() = default;
      virtual size_t getNumberOfNeuronsPrv() const = 0;
      virtual size_t getNumberOfNeuronsCur() const = 0;
      virtual size_t getNumberOfParameters() const = 0;
      virtual void setActivation(std::span<const double> outputPrv, std::span<const double> params, std::span<double> activation) const = 0;
      virtual void backpropagateError(std::span<const double> error, std::span<const double> params, std::span<double> errorPrv) const = 0;
      virtual void backpropagateParamDeriv(std::span<const double> errorCur, std::span<const double> outputPrv, std::span<double> dCost_dParam) const = 0;
   };
}
