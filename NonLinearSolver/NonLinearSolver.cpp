#include "NonLinearSolver.h"
#include "MatrixSquare.h"
#include "INonLinearSolverStrategy.h"
#include "IRealFunction.h"
#include "IRealFunctionUtils.h"
#include "MyAssert.h"

#include <algorithm>
#include <vector>


NonLinearSolver::NonLinearSolver(IRealFunction& problem) :
	m_problem(problem)
{
	Utilities::MyAssert(problem.GetRangeDimension() == problem.GetDomainDimension());
}

NonLinearSolverStatus NonLinearSolver::Solve(std::span< const double> fx, std::span<  double>x, INonLinearSolverStrategy& strategy)
{
	const auto dim = fx.size();
	Utilities::MyAssert(x.size() == dim);
	Utilities::MyAssert(GetDimension(m_problem) == dim);
	
	std::vector<double> rsd(dim);
	std::vector<double> cor(dim);
	MatrixSquare jacobian(static_cast<int>(dim));
	
	// Calculate initial residual
	m_problem.Evaluate(x, rsd);
	std::transform(fx.begin(), fx.end(), rsd.begin(), rsd.begin(), std::minus<double>());
	
	int iter = 0;
	auto status = strategy.GetStatus(iter, rsd);

	while (status == NonLinearSolverStatus::NotConverged)
	{
		// Calculate correction
		m_problem.Derivative(x, jacobian);
		bool succes = jacobian.Solve(rsd, cor);
		if (!succes)
		{
			// Panick, exit
			return NonLinearSolverStatus::Diverged;
		}

		// Apply correction
		std::transform(x.begin(), x.end(), cor.begin(), x.begin(), std::plus<double>());		

		// Calculate residual
		m_problem.Evaluate(x, rsd);
		std::transform(fx.begin(), fx.end(), rsd.begin(), rsd.begin(), std::minus<double>());

		// Check convergence
		++iter;
		status = strategy.GetStatus(iter, rsd);
	}
	return status;
}