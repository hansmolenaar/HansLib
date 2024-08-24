#pragma once

#include "IAnnWeights.h"

namespace ML
{
   class AnnWeightsNoBias : public IAnnWeights
   {
      // Storage
      //
      //    l-1                           l
      //
      //     n_1           -             n_3
      //                   x
      //     n_2           -             n_4
      //
      // { n_13, n 23, n_41, n_42 }
   public:
      AnnWeightsNoBias(size_t dimPrv, size_t dimCur);
      size_t getNumberOfNeuronsPrv() const override;
      size_t getNumberOfNeuronsCur() const override;
      size_t getNumberOfParameters() const override;
      void transform(std::span<const double> activatorValuesPrv, std::span<const double> params, std::span<double> weightedAverage) const override;
      void backpropInit(std::span<const double> activatorValuesPrv, std::span<const double> dError_dWeightedAverageLast, std::span<double> dError_dParam) const override;
      void backpropagateError(std::span<const double> error, std::span<const double> params, std::span<double> errorPrv) const override;
      void backpropagateParamDeriv(std::span<const double> errorCur, std::span<const double> activatorValuesPrv, std::span<double> dError_dParam) const override;
   private:
      size_t m_layerSizePrv;
      size_t m_layerSizeCur;
   };
}
