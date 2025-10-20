#pragma once

#include "NonLinearSolverStatus.h"
#include <span>

class IRealFunction;
class INonLinearSolverStrategy;

class NonLinearSolver
{
  public:
    NonLinearSolver(IRealFunction &problem);
    NonLinearSolverStatus Solve(std::span<const double> fx, std::span<double> x, INonLinearSolverStrategy &strategy);

  private:
    IRealFunction &m_problem;
};
