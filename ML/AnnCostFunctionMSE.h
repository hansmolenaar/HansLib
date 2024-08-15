#pragma once

#include "IAnnCostFunction.h"

namespace ML
{
   // Mean qqared error
   class AnnCostFunctionMSE : public IAnnCostFunction
   {
   public:
      double calculateSingleSample(std::span<const double> ideal, std::span<const double> actual) const override;
      double calculate(const IAnnDataSet& ideal, std::span< std::span<const double>> actual) const override;
   };
}
