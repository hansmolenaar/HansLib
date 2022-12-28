#pragma once

#include "HierarchicalBasisFunction.h"
#include <span>

class HierarchicalApproximationFunction
{
public:
   explicit HierarchicalApproximationFunction(HierarchicalMultiIndex&&, double);
   double operator()(std::span<const double> x) const;
   size_t getDimension() const;
private:
   HierarchicalBasisFunction m_basisFunction;
   double m_coefficient;
};