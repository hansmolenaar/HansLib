#pragma once
#include "HierarchicalApproximationFunction.h"

#include <memory>
#include <span>
#include <map>

class ISingleVariableRealValuedFunction;

struct ValueSurplus
{
   double Value;
   double Surplus;
};

class HierarchicalApproximation1D
{
public:
   static std::unique_ptr<HierarchicalApproximation1D> CreateToLevel(const ISingleVariableRealValuedFunction& fie, size_t level);
   double operator()(std::span<const double> x) const;
   size_t getDimension() const;
private:
   HierarchicalApproximation1D() = default;
   std::map<HierarchicalLevelIndex, ValueSurplus> m_functions;
};