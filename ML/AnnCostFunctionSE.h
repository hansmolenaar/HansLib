#pragma once

#include "IAnnCostFunction.h"

namespace ML
{
   // Sqared error (but not averaged, that is AnnCostFunctionMSE)
   class AnnCostFunctionSE : public IAnnCostFunction
   {
   public:
      double calculateSingleSample(std::span<const double> ideal, std::span<const double> actual) const override;
      double calculate(const IAnnDataSet& ideal, std::span< std::span<const double>> actual) const override;
      double getScaleFactor(const ML::IAnnDataSet& ideal) const override;
   };
}
