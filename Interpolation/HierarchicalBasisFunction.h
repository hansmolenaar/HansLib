#pragma once

#include "HierarchicalMultiIndex.h"
#include <span>

class HierarchicalBasisFunction
{
public:
   explicit HierarchicalBasisFunction(HierarchicalMultiIndex&&);
   double evaluate(std::span<const double> x) const;
   size_t getDimension() const;
private:
   HierarchicalMultiIndex m_hierIndex;
};