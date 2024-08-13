#pragma once

#include "AnnWeightedAverageMatrix.h"

namespace ML
{
   class AnnWeightedAverageSingleBias : public IAnnWeightedAverage
   {
   public:
      AnnWeightedAverageSingleBias(size_t dimPrv, size_t dimCur);
      size_t getNumberOfNeuronsPrv() const override;
      size_t getNumberOfNeuronsCur() const override;
      size_t getNumberOfParameters() const override;
      void transform(std::span<const double> activatorValuesPrv, std::span<const double> params, std::span<double> weightedAverage) const override;
   private:
      AnnWeightedAverageMatrix m_matrixOnly;
   };
}
