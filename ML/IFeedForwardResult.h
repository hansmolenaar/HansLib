#pragma once

#include <span>

namespace ML
{
class IFeedForwardResult
{
  public:
    virtual ~IFeedForwardResult() = default;

    // Note input is not stored, level 0 is the first hidden (or output) layer
    virtual std::span<const double> getActivationAt(size_t n) const = 0;
    virtual std::span<const double> getOutputAt(size_t n) const = 0;
    virtual std::span<const double> getOutput() const = 0;
    virtual std::span<const double> getInput() const = 0;
};
} // namespace ML
