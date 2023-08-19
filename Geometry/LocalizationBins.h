#pragma once

#include "Interval.h"
#include <vector>
#include <span>

class LocalizationBins
{
public:
   static LocalizationBins CreateUniform(const Interval<double>& inverval, int nBins);
   static LocalizationBins CreateFromValues(std::vector< double> values, bool strict, double minBinSize = std::numeric_limits<double>::epsilon());

   // throws if not in any bin
   size_t find(double x) const;

   size_t getNumBins() const;
   double getBinLower(size_t n) const;
   double getBinUpper(size_t n) const;

private:
   explicit LocalizationBins(std::vector<double> vertices);

   std::vector<double> m_vertices;
};
