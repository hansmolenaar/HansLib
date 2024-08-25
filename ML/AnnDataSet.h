#pragma once

#include "IAnnDataSet.h"
#include <initializer_list>
#include <vector>

namespace ML
{
   class AnnDataSet : public IAnnDataSet
   {
   public:
      AnnDataSet(size_t sizeIn, size_t sizeOut);
      AnnDataSet(std::span<const double> input, std::span<const double> output);

      void addSample(std::initializer_list<double> input, std::initializer_list<double> output);
      void addSample(std::span<const double> input, std::span<const double> output);

      size_t getNumberOfSamples() const override;
      std::span<const double> getNthInput(size_t n) const override;
      std::span<const double> getNthOutput(size_t n) const override;
   private:
      size_t m_sizeIn;
      size_t m_sizeOut;
      size_t m_sizeInOut;
      std::vector<double> m_allSamples;
   };
}
