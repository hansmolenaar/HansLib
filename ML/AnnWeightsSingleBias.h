#pragma once

#include "AnnWeightsNoBias.h"

namespace ML
{
   class AnnWeightsSingleBias : public IAnnWeights
   {
   public:
      AnnWeightsSingleBias(size_t dimPrv, size_t dimCur);
      size_t getNumberOfNeuronsPrv() const override;
      size_t getNumberOfNeuronsCur() const override;
      size_t getNumberOfParameters() const override;
      // Storage params: first the matrix params and then the single bias
      void setActivation(std::span<const double> outputPrv, std::span<const double> params, std::span<double> activation)  const override;

      void backpropagateError(std::span<const double> error, std::span<const double> params, std::span<double> errorPrv) const override;
      void backpropagateParamDeriv(std::span<const double> errorCur, std::span<const double> outputPrv, std::span<double> dCost_dParam) const override;
   private:
      AnnWeightsNoBias m_matrixOnly;
   };
}
