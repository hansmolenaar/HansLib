#include "HierBasisFunction.h"
#include "IHierBasisFunction1D.h"

#include "Utilities/Defines.h"
#include "Utilities/Assert.h"

namespace
{
   HierMultiIndex CreateMultiIndex(const std::vector<const IHierBasisFunction1D*>& fies)
   {
      std::vector<HierLevelIndex> multiIndex;
      str::transform(fies, std::back_inserter(multiIndex), [](const IHierBasisFunction1D* fie) {return fie->getLevelIndex(); });
      return HierMultiIndex(std::move(multiIndex));
   }
}

HierBasisFunction::HierBasisFunction(const std::vector<const IHierBasisFunction1D*>& fies) :
   m_basisFunctions(fies), m_multiIndex(CreateMultiIndex(m_basisFunctions))
{
}

const HierMultiIndex& HierBasisFunction::getMultiIndex() const
{
   return m_multiIndex;
}

const std::vector<const IHierBasisFunction1D*>& HierBasisFunction::getBasisFunctions1D() const
{
   return m_basisFunctions;
}

double HierBasisFunction::operator()(std::span<const double> x) const
{
   Utilities::Assert(m_basisFunctions.size() == x.size());
   double result = 1.0;
   for (size_t n = 0; n < m_basisFunctions.size(); ++n)
   {
      result *= m_basisFunctions.at(n)->Evaluate(x[n]);
   }
   return result;
}