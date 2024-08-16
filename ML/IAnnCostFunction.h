#pragma once

#include <cstddef>
#include <span>

namespace ML
{
   class IAnnDataSet;

   // Artificial Neural Network, cost-function
   class IAnnCostFunction
   {
   public:
      virtual ~IAnnCostFunction() = default;
      virtual double calculateSingleSample(std::span<const double> ideal, std::span<const double> actual) const = 0;
      virtual double calculate(const ML::IAnnDataSet& ideal, std::span< std::span<const double>> actual) const = 0;
      virtual double getScaleFactor(const ML::IAnnDataSet& ideal) const = 0;
   };
}
