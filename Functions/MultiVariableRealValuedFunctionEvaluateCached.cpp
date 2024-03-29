#include "MultiVariableRealValuedFunctionEvaluateCached.h"

MultiVariableRealValuedFunctionEvaluateCached::MultiVariableRealValuedFunctionEvaluateCached(std::shared_ptr<IMultiVariableFunctionEvaluate> fie) :
   m_function(fie)
{
}

int MultiVariableRealValuedFunctionEvaluateCached::getDimension() const
{
   return m_function->getDimension();
}

double MultiVariableRealValuedFunctionEvaluateCached::operator()(std::span<const double> x) const
{
   const std::vector<double> values(x.begin(), x.end());
   const auto found = m_cache.find(values);
   if (found != m_cache.end())
   {
      ++m_numCacheHits;
      return found->second;
   }

   ++m_numEvaluations;
   const double eval = (*m_function)(x);
   m_cache[values] = eval;
   return eval;
}