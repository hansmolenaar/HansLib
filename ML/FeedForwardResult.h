#pragma once

#include "IFeedForwardResult.h"
#include "AnnArray.h"

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
      ML::AnnArray  m_outputs;
      ML::AnnArray  m_averages;
   };
}
