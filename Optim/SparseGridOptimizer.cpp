#include "SparseGridOptimizer.h"
#include "NodeRefinePredicateNever.h"


namespace
{
   IterativeMinimizationStep GetMinimumOfNewNodes(const HierApproximation& approximation, std::vector<const HierTreeNode*> newNodes)
   {
      IterativeMinimizationStep result;
      for (const auto* newNode : newNodes)
      {
         const auto eval = newNode->Value;
         if (eval < result.EvalMinimum)
         {
            result.EvalMinimum = eval;
            result.ArgMinimum = newNode->getMultiIndex().toDoubles();
         }
      }
      return result;
   }
  
}

SparseGridOptimizer::SparseGridOptimizer(std::shared_ptr<IMultiVariableFunctionEvaluate>function, int maxLevel) : 
   m_basisFunctionFactory(function->getDimension(), &m_basisFunction1DFactory),
   m_objectiveFunction(function), 
   m_maxLevel(maxLevel)
{
   NodeRefinePredicateFactoryNever predicate;
   m_approximation = HierApproximation::Create(*function, m_basisFunctionFactory, predicate);
   m_steps.emplace_back(GetMinimumOfNewNodes(*m_approximation, m_approximation->getAllTreeNodesRO()));
   m_steps.back().Status = StepSucces;
}

const IMultiVariableFunctionEvaluate& SparseGridOptimizer::getObjectiveFunction() const
{
   return *m_objectiveFunction;
}