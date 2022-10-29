#include "NonLinearSolver/NonLinearSolverStrategyResidualReduction.h"

#include <cassert>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <string>

NonLinearSolverStrategyResidualReduction::NonLinearSolverStrategyResidualReduction(const NonLinearSolverConvergenceCriteria& crit) :
   m_convergenceCrit(crit)
{
}

NonLinearSolverStatus NonLinearSolverStrategyResidualReduction::GetStatus(int iter, std::span<const  double> rsd)
{
   if (iter != m_residuals.size())
   {
      const std::string msg = "NonLinearSolverStrategyResidualReduction::GetStatus rsd should have size " + std::to_string(iter) + ", but is " + std::to_string(m_residuals.size());
      throw std::exception(msg.c_str());
   }
   const double normResidual = std::sqrt(std::inner_product(rsd.begin(), rsd.end(), rsd.begin(), 0.0));
   m_residuals.push_back(normResidual);
   if (iter == 0)
   {
      // Sick case
      if (m_residuals[0] == 0.0)
      {
         return NonLinearSolverStatus::Converged;
      }
   }
   else if (iter > m_convergenceCrit.GetMaxIter())
   {
      return NonLinearSolverStatus::MaxIterExceeded;
   }
   else if (normResidual < m_convergenceCrit.GetResidualReduction() * m_residuals[0])
   {
      return NonLinearSolverStatus::Converged;
   }
   else if (normResidual > 1.0e3 * m_residuals[0])
   {
      return NonLinearSolverStatus::Diverged;
   }
   return NonLinearSolverStatus::NotConverged;
}

int NonLinearSolverStrategyResidualReduction::GetNumIterations() const
{
   return static_cast<int>(m_residuals.size()) - 1;
}

void NonLinearSolverStrategyResidualReduction::Reset()
{
   m_residuals.clear();
}