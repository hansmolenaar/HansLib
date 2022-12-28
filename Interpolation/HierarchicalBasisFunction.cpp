#include "HierarchicalBasisFunction.h"
#include "HierarchicalBasisFunction1D.h"
#include "Utilities/Assert.h"
#include "Utilities/Defines.h"

HierarchicalBasisFunction::HierarchicalBasisFunction(HierarchicalMultiIndex&& mi) : m_hierIndex(std::move(mi))
{}

size_t HierarchicalBasisFunction::getDimension() const
{
   return m_hierIndex.getDimension();
}

double HierarchicalBasisFunction::operator()(std::span<const double> x) const
{
   Utilities::Assert(x.size() == getDimension());
   const auto loop = stv::iota(static_cast<size_t>(0), x.size());
   return std::accumulate(loop.begin(), loop.end(), 1.0, [this, &x](double acc, size_t n) {return acc * HierarchicalBasisFunction1D(m_hierIndex.get().at(n))(x[n]); });
}