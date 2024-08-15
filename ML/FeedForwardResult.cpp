#include "FeedForwardResult.h"

#include "MyAssert.h"

ML::FeedForwardResult::FeedForwardResult(std::span<const size_t> dimensions) : m_offset(dimensions.size() + 1)
{
   size_t totalSize = 0;
   m_offset[0] = 0;
   for (size_t n = 0; n < dimensions.size(); ++n)
   {
      m_offset[n + 1] = m_offset[n] + dimensions[n];
      totalSize += dimensions[n];
   }
   m_averages.resize(totalSize);
   m_outputs.resize(totalSize);
}

size_t ML::FeedForwardResult::getLayerSizeAt(size_t n) const
{
   return m_offset.at(n + 1) - m_offset.at(n);
}

std::span< double> ML::FeedForwardResult::setWeightedInputAt(size_t n)
{
   return std::span<double>(m_averages.begin() + m_offset.at(n), getLayerSizeAt(n));
}

std::span< double> ML::FeedForwardResult::setOutputAt(size_t n)
{
   return std::span<double>(m_outputs.begin() + m_offset.at(n), getLayerSizeAt(n));
}

std::span<const double> ML::FeedForwardResult::getWeightedInputAt(size_t n) const
{
   return std::span<const double>(m_averages.begin() + m_offset.at(n), getLayerSizeAt(n));
}

std::span<const double> ML::FeedForwardResult::getOutputAt(size_t n) const
{
   return std::span<const double>(m_outputs.begin() + m_offset.at(n), getLayerSizeAt(n));
}

std::span<const double> ML::FeedForwardResult::getOutput() const
{
   return getOutputAt(m_offset.size() - 2);
}