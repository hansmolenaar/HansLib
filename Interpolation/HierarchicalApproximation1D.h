#pragma once
#include "HierarchicalApproximationFunction.h"

#include <memory>
#include <span>
#include <array>

class ISingleVariableRealValuedFunction;

class HierarchicalApproximation1D
{
public:
   static std::unique_ptr<HierarchicalApproximation1D> CreateToLevel(const ISingleVariableRealValuedFunction& fie, size_t level);
   double operator()(std::span<const double> x) const;
   size_t getDimension() const;
private:
   HierarchicalApproximation1D() = default;
   std::vector<HierarchicalApproximationFunction> m_functions;
};