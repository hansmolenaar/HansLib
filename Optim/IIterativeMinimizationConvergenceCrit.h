#pragma once
#include "IterativeMinimizationStep.h"
#include <array>
#include <vector>

enum IterativeMinimizationConvergenceStatus
{
    Converged,
    NotConverged,
    MaxIterExceeded,
    Fail
};
constexpr std::array<IterativeMinimizationConvergenceStatus, 4> IterativeMinimizationConvergenceStatusAll = {
    Converged, NotConverged, MaxIterExceeded, Fail};

class IIterativeMinimizationConvergenceCrit
{
  public:
    virtual ~IIterativeMinimizationConvergenceCrit() noexcept = default;
    virtual IterativeMinimizationConvergenceStatus getConvergenceStatus(
        const std::vector<IterativeMinimizationStep> &) const = 0;
};