#pragma once

#include "IMultiVariableFunctionEvaluate.h"
#include <memory>

class MultiVariableFunctionEvaluateFlipped : public IMultiVariableFunctionEvaluate
{
public:
   MultiVariableFunctionEvaluateFlipped(std::unique_ptr<IMultiVariableFunctionEvaluate>&& fie);

   int getDimension() const override;
   double operator()(std::span<const double> x) const override;

private:
   std::unique_ptr<IMultiVariableFunctionEvaluate> m_orgFunction;
};