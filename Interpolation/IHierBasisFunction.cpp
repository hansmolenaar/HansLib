#include "IHierBasisFunction.h"
#include "Defines.h"
#include "IHierBasisFunction1D.h"
#include "MyAssert.h"

bool IHierBasisFunction::contains(std::span<const double> x) const
{
    const auto &basis = getBasisFunctions1D();
    Utilities::MyAssert(basis.size() == x.size());
    for (size_t n = 0; n < x.size(); ++n)
    {
        if (!basis.at(n)->getSupport().contains(x[n]))
            return false;
    }
    return true;
}