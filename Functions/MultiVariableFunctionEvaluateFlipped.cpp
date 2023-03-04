#include "MultiVariableFunctionEvaluateFlipped.h"

MultiVariableFunctionEvaluateFlipped::MultiVariableFunctionEvaluateFlipped(std::shared_ptr<IMultiVariableFunctionEvaluate> fie) : 
   m_orgFunction(fie)
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