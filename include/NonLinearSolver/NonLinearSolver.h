#pragma once

#include <span>
#include "NonLinearSolverStatus.h"

class IRealFunction;
class INonLinearSolverStrategy;

class NonLinearSolver
{
public:
	NonLinearSolver(IRealFunction& problem);
	NonLinearSolverStatus Solve(std::span< const double> fx, std::span<  double>x, INonLinearSolverStrategy& strategy);
private:
	IRealFunction& m_problem;
};
