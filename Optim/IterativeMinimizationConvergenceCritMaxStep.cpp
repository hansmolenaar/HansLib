#include "IterativeMinimizationConvergenceCritMaxStep.h"

IterativeMinimizationConvergenceCritMaxStep::IterativeMinimizationConvergenceCritMaxStep(int maxStep)
    : m_maxStep(maxStep)
{
}

IterativeMinimizationConvergenceStatus IterativeMinimizationConvergenceCritMaxStep::getConvergenceStatus(
    const std::vector<IterativeMinimizationStep> &steps) const
{
    // Note, this is tested after refinement.
    if (static_cast<int>(steps.size()) + 1 > m_maxStep)
    {
        return MaxIterExceeded;
    }
    return NotConverged;
}
