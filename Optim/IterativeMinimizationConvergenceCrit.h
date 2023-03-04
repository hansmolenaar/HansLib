#pragma once

#include "IIterativeMinimizationConvergenceCrit.h"

class IterativeMinimizationConvergenceCrit : public IIterativeMinimizationConvergenceCrit
{
public:
   IterativeMinimizationConvergenceCrit(size_t, double);
    IterativeMinimizationConvergenceStatus getConvergenceStatus(const std::vector<IterativeMinimizationStep>&) const override;
private:
   size_t m_maxNumIterations;
   double m_evalLessThan;
};