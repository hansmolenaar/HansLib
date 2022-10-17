#pragma once

#include <gsl/gsl-lite.hpp>
#include "INonLinearSolverStrategy.h"
#include "NonLinearSolverConvergenceCriteria.h"

class NonLinearSolverStrategyResidualReduction : public INonLinearSolverStrategy
{
public:
	NonLinearSolverStrategyResidualReduction(const NonLinearSolverConvergenceCriteria&);
	NonLinearSolverStatus GetStatus(int iter, std::span<const  double> rsd)  override;
	void Reset() override;
	int GetNumIterations() const;

private:
   NonLinearSolverConvergenceCriteria m_convergenceCrit;
	std::vector<double> m_residuals;
};
