#include "NonLinearSolver/NonLinearSolverConvergenceCriteria.h"

NonLinearSolverConvergenceCriteria::NonLinearSolverConvergenceCriteria(double reduction, int maxIter) :
	m_rsdReduction(reduction), m_maxIter(maxIter)
{
}
double NonLinearSolverConvergenceCriteria::GetResidualReduction() const
{
   return m_rsdReduction;
}

int NonLinearSolverConvergenceCriteria::GetMaxIter() const
{
   return m_maxIter;
}