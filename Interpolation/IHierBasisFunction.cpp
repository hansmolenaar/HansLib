#include "IHierBasisFunction.h"
#include "Utilities/Assert.h"
#include "Utilities/Defines.h"

bool IHierBasisFunction::contains(std::span<const double> x) const
{
   Utilities::Assert(GetDomainDimension() == x.size());
   for (size_t n = 0; n < x.size(); ++n)
   {
      if (!getSupportInDirection(n).contains(x[n])) return false;
   }
   return true;
}