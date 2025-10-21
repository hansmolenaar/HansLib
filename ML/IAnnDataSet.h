#pragma once

#include <span>

namespace ML
{
class IAnnDataSet
{
  public:
    virtual ~IAnnDataSet() = default;
    virtual size_t getNumberOfSamples() const = 0;
    virtual std::span<const double> getNthInput(size_t n) const = 0;
    virtual std::span<const double> getNthOutput(size_t n) const = 0;
};
} // namespace ML
