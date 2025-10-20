#pragma once

#include "Interval.h"
#include "MultiVariableRealValuedFunctionEvaluateCached.h"

struct MinimumInfo
{
    double Extremum = std::numeric_limits<double>::max();
    std::vector<std::vector<double>> Positions;
};

struct MinimizationProblem
{
    std::shared_ptr<MultiVariableRealValuedFunctionEvaluateCached> Function;
    std::vector<Interval<double>> Domain;
    MinimumInfo Minimum;
};

class MinimizationProblemOnUnitDomain
{
  public:
    explicit MinimizationProblemOnUnitDomain(MinimizationProblem problem);
    const MinimizationProblem &getOriginalProblem() const;
    const MinimumInfo &getMinimumOnUnitInfo() const;
    std::shared_ptr<IMultiVariableFunctionEvaluate> getFunctionOnUnit() const;

  private:
    MinimizationProblem m_originalProblem;
    MinimumInfo m_minimumOnUnitDomain;
    std::shared_ptr<IMultiVariableFunctionEvaluate> m_functionOnUnit;
};

namespace MinimizationExamples
{
MinimizationProblem Square1D();
MinimizationProblemOnUnitDomain Square2D();
MinimizationProblemOnUnitDomain SixHumpCamelFunction();
MinimizationProblemOnUnitDomain Rosenbrock2();
} // namespace MinimizationExamples