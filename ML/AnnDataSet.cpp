#include "AnnDataSet.h"

#include "MyAssert.h"

ML::AnnDataSet::AnnDataSet(size_t sizeIn, size_t sizeOut) : m_sizeIn(sizeIn), m_sizeOut(sizeOut), m_sizeInOut(sizeIn + sizeOut)
{
}

ML::AnnDataSet::AnnDataSet(std::span<const double> input, std::span<const double> output) : AnnDataSet(input.size(), output.size())
{
   addSample(input, output);
}

void ML::AnnDataSet::addSample(std::span<const double> input, std::span<const double> output)
{
   Utilities::MyAssert(input.size() == m_sizeIn);
   Utilities::MyAssert(output.size() == m_sizeOut);
   m_allSamples.insert(m_allSamples.end(), input.begin(), input.end());
   m_allSamples.insert(m_allSamples.end(), output.begin(), output.end());
}

void ML::AnnDataSet::addSample(std::initializer_list<double> input, std::initializer_list<double> output)
{
   addSample(std::span<const double>(input), std::span<const double>(output));
}

size_t ML::AnnDataSet::getNumberOfSamples() const
{
   return m_allSamples.size() / m_sizeInOut;
}

std::span<const double> ML::AnnDataSet::getNthInput(size_t n) const
{
   Utilities::MyAssert(n < getNumberOfSamples());
   return std::span(m_allSamples.begin() + n * m_sizeInOut, m_sizeIn);
}

std::span<const double> ML::AnnDataSet::getNthOutput(size_t n) const
{
   Utilities::MyAssert(n < getNumberOfSamples());
   return std::span(m_allSamples.begin() + n * m_sizeInOut + m_sizeIn, m_sizeOut);
}