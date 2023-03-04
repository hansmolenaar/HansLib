#pragma once

#include "IMultiVariableFunctionEvaluate.h"
#include "Functors.h"
#include <memory>
#include <map>
#include <vector>

class MultiVariableRealValuedFunctionEvaluateCached : public IMultiVariableFunctionEvaluate
{
public:
   explicit MultiVariableRealValuedFunctionEvaluateCached(std::shared_ptr<IMultiVariableFunctionEvaluate> fie);

   int getDimension() const override;
   double operator()(std::span<const double> x) const override;

   int getNumEvaluations() const { return m_numEvaluations; }
   int getNumCacheHits() const { return m_numCacheHits; }

private:
   std::shared_ptr< IMultiVariableFunctionEvaluate> m_function;
   mutable std::map<std::vector<double>, double, Functors::VectorDoubleLess> m_cache;
   mutable int m_numEvaluations = 0;
   mutable  int m_numCacheHits = 0;
};