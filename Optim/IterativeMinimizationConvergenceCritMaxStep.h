#pragma once

#include "IIterativeMinimizationConvergenceCrit.h"

class IterativeMinimizationConvergenceCritMaxStep : public IIterativeMinimizationConvergenceCrit
{
public:
	explicit IterativeMinimizationConvergenceCritMaxStep(int maxStep);
	IterativeMinimizationConvergenceStatus getConvergenceStatus(const std::vector<IterativeMinimizationStep>&) const override;
private:
	int m_maxStep;
};