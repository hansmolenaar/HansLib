#pragma once

#include "AnnWeightedAverageMatrix.h"

namespace ML
{
   class AnnWeightedAverageWithBias : public IAnnWeightedAverage
   {
   public:
      AnnWeightedAverageWithBias(size_t dimPrv, size_t dimCur);
      size_t getNumberOfNeuronsPrv() const override;
      size_t getNumberOfNeuronsCur() const override;
      size_t getNumberOfParameters() const override;
      // Storage params: first the matrix params and then the single bias
      void transform(std::span<const double> activatorValuesPrv, std::span<const double> params, std::span<double> weightedAverage) const override;

      void backpropInit(std::span<const double> activatorValuesPrv, std::span<const double> dError_dWeightedAverageLast, std::span<double> dError_dParam) const override;
   private:
      AnnWeightedAverageMatrix m_matrixOnly;
   };
}
