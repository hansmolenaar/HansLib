#include "FeedForwardResult.h"

#include "MyAssert.h"

ML::FeedForwardResult::FeedForwardResult(std::span<const size_t> dimensions) : m_outputs(dimensions), m_averages(dimensions)
{
}

std::span< double> ML::FeedForwardResult::setWeightedInputAt(size_t n)
{
   return m_averages.modifyValuesAt(n);
}

std::span< double> ML::FeedForwardResult::setOutputAt(size_t n)
{
   return m_outputs.modifyValuesAt(n);
}

std::span<const double> ML::FeedForwardResult::getWeightedInputAt(size_t n) const
{
   return m_averages.getValuesAt(n);
}

std::span<const double> ML::FeedForwardResult::getOutputAt(size_t n) const
{
   return m_outputs.getValuesAt(n);
}

std::span<const double> ML::FeedForwardResult::getOutput() const
{
   return getOutputAt(m_outputs.getNumLayers() - 1);
}