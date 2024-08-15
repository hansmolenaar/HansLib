#include "AnnCostFunctionSE.h"
#include "IAnnDataSet.h"
#include "MyAssert.h"

double ML::AnnCostFunctionSE::calculateSingleSample(std::span<const double> ideal, std::span<const double> actual) const
{
   Utilities::MyAssert(ideal.size() == actual.size());
   double result = 0;
   for (size_t n = 0; n < ideal.size(); ++n)
   {
      const double d = ideal[n] - actual[n];
      result += d * d;
   }
   return 0.5 * result;
}

double ML::AnnCostFunctionSE::calculate(const IAnnDataSet& ideal, std::span< std::span<const double>> actual) const
{
   Utilities::MyAssert(!actual.empty());
   Utilities::MyAssert(ideal.getNumberOfSamples() == actual.size());
   double result = 0;
   for (size_t n = 0; n < actual.size(); ++n)
   {
      result += calculateSingleSample(ideal.getNthOutput(n), actual[n]);
   }
   return result;
}