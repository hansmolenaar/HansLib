#include "MultiVariableFunctionEvaluateFlipped.h"

MultiVariableFunctionEvaluateFlipped::MultiVariableFunctionEvaluateFlipped(std::unique_ptr<IMultiVariableFunctionEvaluate>&& fie) : 
   m_orgFunction(std::move(fie))
{
}

int MultiVariableFunctionEvaluateFlipped::getDimension() const
{
   return m_orgFunction->getDimension();
}

double MultiVariableFunctionEvaluateFlipped::operator()(std::span<const double> x) const
{
   return -(*m_orgFunction)(x);
}