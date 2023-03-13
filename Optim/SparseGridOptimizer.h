#pragma once

#include "IMultiVariableFunctionEvaluate.h"
#include "IIterativeMinimization.h"
#include "HierApproximation.h"
#include "HierBasisFunction1D_ExtendedLevelOneBC.h"
#include "HierBasisFunction.h"

#include <memory>

class SparseGridOptimizer : public IIterativeMinimization
{
public:
	SparseGridOptimizer(std::shared_ptr<IMultiVariableFunctionEvaluate>function, INodeRefinePredicateFactory& predicateFactory);

	IterativeMinimizationStep iterate() override;
	const IMultiVariableFunctionEvaluate& getObjectiveFunction() const override;

	const std::vector< IterativeMinimizationStep>& getAllSteps() const;

private:
	HierBasisFunction1D_ExtendedLevelOneBC_Factory m_basisFunction1DFactory;
	HierBasisFunction_Factory m_basisFunctionFactory;
	std::shared_ptr<IMultiVariableFunctionEvaluate> m_objectiveFunction;
	INodeRefinePredicateFactory& m_predicateFactory;
	std::unique_ptr<HierApproximation> m_approximation;
	std::vector< IterativeMinimizationStep> m_steps;
};