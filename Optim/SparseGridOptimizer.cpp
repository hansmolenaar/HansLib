#include "SparseGridOptimizer.h"
#include "NodeRefinePredicateNever.h"
#include "Defines.h"


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

   bool Compare(const HierTreeNode* a, const HierTreeNode* b)
   {
      return a->RefinementLevel < b->RefinementLevel;
   }

   int GetMaxLevel(std::vector<const HierTreeNode*> hierNodes)
   {
      return (*str::max_element(hierNodes, Compare))->RefinementLevel;
   }
}

SparseGridOptimizer::SparseGridOptimizer(std::shared_ptr<IMultiVariableFunctionEvaluate>function, INodeRefinePredicateFactory& predicateFactory) :
   m_basisFunctionFactory(function->getDimension(), &m_basisFunction1DFactory),
   m_objectiveFunction(function),
   m_predicateFactory(predicateFactory)
{
   // Always create lowest level
   NodeRefinePredicateFactoryNever predicate;
   m_approximation = HierApproximation::Create(*function, m_basisFunctionFactory, predicate);
   m_steps.emplace_back(GetMinimumOfNewNodes(*m_approximation, m_approximation->getAllTreeNodesRO()));
   m_steps.back().Status = StepSucces;
}

const IMultiVariableFunctionEvaluate& SparseGridOptimizer::getObjectiveFunction() const
{
   return *m_objectiveFunction;
}

const std::vector< IterativeMinimizationStep>& SparseGridOptimizer::getAllSteps() const
{
   return m_steps;
}

IterativeMinimizationStep SparseGridOptimizer::iterate()
{
   const auto justRefined = m_approximation->iterate(m_predicateFactory);
   if (justRefined.empty())
   {
      return m_steps.back();
   }
   auto result = GetMinimumOfNewNodes(*m_approximation, justRefined);
   result.Status = StepSucces;
   return result;
}

const HierApproximation& SparseGridOptimizer::getApproximation() const
{
   return *m_approximation;
}