#include "HierarchicalApproximationFunction.h"

HierarchicalApproximationFunction::HierarchicalApproximationFunction(HierarchicalMultiIndex&& index, double coefficient) :
   m_basisFunction(std::move(index)), m_coefficient(coefficient)
{}

size_t HierarchicalApproximationFunction::getDimension() const
{
   return m_basisFunction.getDimension();
}

double HierarchicalApproximationFunction::operator()(std::span<const double> x) const
{
   return  m_coefficient * m_basisFunction(x);
}
