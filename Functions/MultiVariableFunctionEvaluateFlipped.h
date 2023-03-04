#pragma once

#include "IMultiVariableFunctionEvaluate.h"
#include <memory>

class MultiVariableFunctionEvaluateFlipped : public IMultiVariableFunctionEvaluate
{
public:
   MultiVariableFunctionEvaluateFlipped(std::shared_ptr<IMultiVariableFunctionEvaluate> fie);

   int getDimension() const override;
   double operator()(std::span<const double> x) const override;

private:
   std::shared_ptr<IMultiVariableFunctionEvaluate> m_orgFunction;
};