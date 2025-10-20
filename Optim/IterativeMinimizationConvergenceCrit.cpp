#include "IterativeMinimizationConvergenceCrit.h"

IterativeMinimizationConvergenceCrit::IterativeMinimizationConvergenceCrit(size_t maxIter, double minEval)
    : m_maxNumIterations(maxIter), m_evalLessThan(minEval)
{
}

IterativeMinimizationConvergenceStatus IterativeMinimizationConvergenceCrit::getConvergenceStatus(
    const std::vector<IterativeMinimizationStep> &iterations) const
{
    if (iterations.back().EvalMinimum < m_evalLessThan)
        return Converged;
    if (iterations.size() >= m_maxNumIterations)
        return MaxIterExceeded;
    return NotConverged;
}