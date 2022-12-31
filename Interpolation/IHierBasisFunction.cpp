#include "IHierBasisFunction.h"
#include "Utilities/Assert.h"
#include "Utilities/Defines.h"
#include "IHierBasisFunction1D.h"

bool IHierBasisFunction::contains(std::span<const double> x) const
{
   const auto& basis = getBasisFunctions1D();
   Utilities::Assert(basis.size() == x.size());
   for (size_t n = 0; n < x.size(); ++n)
   {
      if (!basis.at(n)->getSupport().contains(x[n])) return false;
   }
   return true;
}