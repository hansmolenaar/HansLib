#include "SparseGridOptimizer.h"

namespace
{

}

SparseGridOptimizer::SparseGridOptimizer(std::shared_ptr<IMultiVariableFunctionEvaluate>function, int maxLevel) : 
   m_objectiveFunction(function), m_maxLevel(maxLevel)
{
}

const IMultiVariableFunctionEvaluate& SparseGridOptimizer::getObjectiveFunction() const
{
   return *m_objectiveFunction;
}