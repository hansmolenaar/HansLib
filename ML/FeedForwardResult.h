#pragma once

#include "IFeedForwardResult.h"
#include "AnnArray.h"

namespace ML
{
   class FeedForwardResult : public ML::IFeedForwardResult
   {
   public:
      explicit FeedForwardResult(std::span<const double> input, std::span<const size_t> dimensions);
      std::span< double> getActivationAtModifiable(size_t n);
      std::span< double> getOutputAtModifiable(size_t n);
      std::span<const double> getActivationAt(size_t n) const override;
      std::span<const double> getOutputAt(size_t n) const override;
      std::span<const double> getOutput() const override;
      std::span<const double> getInput() const override;
   private:
      ML::AnnArray  m_outputs;
      ML::AnnArray  m_activations;
      std::vector<double> m_input;
   };
}
