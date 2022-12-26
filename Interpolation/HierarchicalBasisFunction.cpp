#include "HierarchicalBasisFunction.h"
#include "HierarchicalBasisFunction1D.h"
#include "Utilities/Assert.h"

HierarchicalBasisFunction::HierarchicalBasisFunction(HierarchicalMultiIndex&& mi) : m_hierIndex(std::move(mi))
{}

size_t HierarchicalBasisFunction::getDimension() const
{
   return m_hierIndex.getDimension();
}

double HierarchicalBasisFunction::evaluate(std::span<const double> x) const
{
   Utilities::Assert(x.size() == getDimension());
   double result = 1;
   for (size_t n = 0; n < x.size(); ++n)
   {
      result *= HierarchicalBasisFunction1D(m_hierIndex.getLevel().at(n), m_hierIndex.getIndex().at(n)).evaluate(x[n]);
   }
   return result;
}