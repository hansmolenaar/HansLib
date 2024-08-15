#pragma once

#include "IFeedForwardResult.h"
#include <vector>

namespace ML
{
   class FeedForwardResult : public ML::IFeedForwardResult
   {
   public:
      explicit FeedForwardResult(std::span<const size_t> dimensions);
      std::span< double> setWeightedInputAt(size_t n);
      std::span< double> setOutputAt(size_t n);
      std::span<const double> getWeightedInputAt(size_t n) const override;
      std::span<const double> getOutputAt(size_t n) const override;
      std::span<const double> getOutput() const override;
   private:
      size_t getLayerSizeAt(size_t n) const;
      std::vector<double>  m_outputs;
      std::vector<double>  m_averages;
      std::vector<size_t>  m_offset;
   };
}
