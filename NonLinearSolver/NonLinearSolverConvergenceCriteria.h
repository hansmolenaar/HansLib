#pragma once

class NonLinearSolverConvergenceCriteria
{
  public:
    NonLinearSolverConvergenceCriteria(double rsdReduction, int maxIter = 1);
    double GetResidualReduction() const;
    int GetMaxIter() const;

  private:
    double m_rsdReduction;
    int m_maxIter;
};
