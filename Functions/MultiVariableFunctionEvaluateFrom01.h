#pragma once

#include "IMultiVariableFunctionEvaluate.h"
#include "Interval.h"
#include <memory>
#include <vector>

class MultiVariableFunctionEvaluateFrom01 : public IMultiVariableFunctionEvaluate
{
public:
   MultiVariableFunctionEvaluateFrom01(std::unique_ptr<IMultiVariableFunctionEvaluate>&& fie, std::vector<Interval<double>>&& targetIntervals);

   int getDimension() const override;
   double operator()(std::span<const double> x) const override;

private:
   std::unique_ptr<IMultiVariableFunctionEvaluate> m_function;
   std::vector<Interval<double>> m_targetIntervals;
};