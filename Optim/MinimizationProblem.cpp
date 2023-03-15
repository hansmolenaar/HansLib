#include "MinimizationProblem.h"
#include "MultiVariablePolynomial.h"
#include "MultiVariableFunctionEvaluateFrom01.h"
#include "MultiVariableFunctionExamples.h"

#include <cmath>


MinimizationProblemOnUnitDomain::MinimizationProblemOnUnitDomain(MinimizationProblem problem) :
   m_originalProblem(std::move(problem))
{
   m_minimumOnUnitDomain = m_originalProblem.Minimum;
   const auto dim = m_originalProblem.Domain.size();
   for (auto& argMin : m_minimumOnUnitDomain.Positions)
   {
      for (size_t d = 0; d < dim; ++d)
      {
         argMin.at(d) = m_originalProblem.Domain.at(d).inverseInterpolate(argMin.at(d));
      }
   }
   std::shared_ptr<IMultiVariableFunctionEvaluate> function = static_pointer_cast<IMultiVariableFunctionEvaluate>(m_originalProblem.Function);
   auto targetIntervals = m_originalProblem.Domain;
   m_functionOnUnit = std::make_shared<MultiVariableFunctionEvaluateFrom01>(function, std::move(targetIntervals));
}

const MinimizationProblem& MinimizationProblemOnUnitDomain::getOriginalProblem() const
{
   return m_originalProblem;
}

std::shared_ptr<IMultiVariableFunctionEvaluate> MinimizationProblemOnUnitDomain::getFunctionOnUnit() const
{
   return m_functionOnUnit;
}

const MinimumInfo& MinimizationProblemOnUnitDomain::getMinimumOnUnitInfo() const
{
   return m_minimumOnUnitDomain;
}

MinimizationProblem MinimizationExamples::Square1D()
{
   const double argmin = 1.0 / 3.0;
   MinimizationProblem result;

   auto square = std::make_shared< MultiVariablePolynomial>(1);
   square->Add(1.0, { 2 });
   square->Add(-2.0 / 3.0, { 1 });
   square->Add(1.0 / 9.0, { 0 });
   result.Function = std::make_shared<MultiVariableRealValuedFunctionEvaluateCached>(square);

   result.Domain.emplace_back(0.0, 1.0);
   result.Minimum.Extremum = 0.0;
   result.Minimum.Positions.emplace_back(std::vector<double>{ argmin});

   return result;
}

MinimizationProblemOnUnitDomain MinimizationExamples::Square2D()
{
   const double argmin0 = 1.0;
   const double argmin1 = 2.0;
   MinimizationProblem result;
   result.Domain.emplace_back(0.0, 3.0);
   result.Domain.emplace_back(0.0, 5.0);
   result.Minimum.Extremum = 0.0;
   result.Minimum.Positions.emplace_back(std::vector<double>{ argmin0, argmin1});

   auto square = std::make_shared< MultiVariablePolynomial>(2);
   square->Add(1.0, { 2, 0 });
   square->Add(-2.0, { 1, 0 });
   square->Add(1.0, { 0, 2 });
   square->Add(-4, { 0, 1 });
   square->Add(5.0, { 0, 0 });
   result.Function = std::make_shared<MultiVariableRealValuedFunctionEvaluateCached>(square);

   return MinimizationProblemOnUnitDomain(result);
}

MinimizationProblemOnUnitDomain MinimizationExamples::SixHumpCamelFunction()
{
   MinimizationProblem result;
   result.Domain.emplace_back(-3.0, 3.0);
   result.Domain.emplace_back(-2.0, 2.0);
   result.Minimum.Extremum = -1.0316;
   result.Minimum.Positions.emplace_back(std::vector<double>{ 0.08983, -0.7126});
   result.Minimum.Positions.emplace_back(std::vector<double>{ -0.08983, 0.7126});

   auto fie = MultiVariableFunctionExamples::SixHumpCamelFunction();
   result.Function = std::make_shared<MultiVariableRealValuedFunctionEvaluateCached>(fie);

   return MinimizationProblemOnUnitDomain(result);
}