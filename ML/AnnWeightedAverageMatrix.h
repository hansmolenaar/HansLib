#pragma once

#include "IAnnWeightedAverage.h"

namespace ML
{
   class AnnWeightedAverageMatrix : public IAnnWeightedAverage
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
      AnnWeightedAverageMatrix(size_t dimPrv, size_t dimCur);
      size_t getNumberOfNeuronsPrv() const override;
      size_t getNumberOfNeuronsCur() const override;
      size_t getNumberOfParameters() const override;
      void transform(std::span<const double> activatorValuesPrv, std::span<const double> params, std::span<double> weightedAverage) const override;
   private:
      size_t m_layerSizePrv;
      size_t m_layerSizeCur;
   };
}
