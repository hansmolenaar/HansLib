#pragma once

#include "IMultiVariableFunctionEvaluate.h"
#include "IIterativeMinimization.h"
#include "HierApproximation.h"
#include <memory>

class SparseGridOptimizer : public IIterativeMinimization
{
public:
	SparseGridOptimizer(std::shared_ptr<IMultiVariableFunctionEvaluate>function, int maxLevel);

	IterativeMinimizationStep iterate() override;
	const IMultiVariableFunctionEvaluate& getObjectiveFunction() const override;

private:
	std::shared_ptr<IMultiVariableFunctionEvaluate> m_objectiveFunction;
	int m_maxLevel;
	std::unique_ptr<HierApproximation> m_approximation;
};