#include "FeedForwardResult.h"

#include "MyAssert.h"

ML::FeedForwardResult::FeedForwardResult(std::span<const double> input, std::span<const size_t> dimensions) : m_outputs(dimensions), m_activations(dimensions), m_input(input.begin(), input.end())
{
}

std::span< double> ML::FeedForwardResult::getActivationAtModifiable(size_t n)
{
   return m_activations.getValuesMutable(n);
}

std::span< double> ML::FeedForwardResult::getOutputAtModifiable(size_t n)
{
   return m_outputs.getValuesMutable(n);
}

std::span<const double> ML::FeedForwardResult::getActivationAt(size_t n) const
{
   return m_activations.getValues(n);
}

std::span<const double> ML::FeedForwardResult::getOutputAt(size_t n) const
{
   return m_outputs.getValues(n);
}

std::span<const double> ML::FeedForwardResult::getOutput() const
{
   return getOutputAt(m_outputs.getNumLayers() - 1);
}

std::span<const double> ML::FeedForwardResult::getInput() const
{
   return m_input;
}
