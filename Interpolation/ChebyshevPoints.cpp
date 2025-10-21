#include "ChebyshevPoints.h"
#include "MathConstants.h"
#include "MyAssert.h"

#include <cmath>

std::vector<double> ChebyshevPoints::Generate(double low, double hig, int nsteps)
{
    Utilities::MyAssert(hig > low);
    Utilities::MyAssert(nsteps > 0);

    const double term = (hig + low) / 2;
    const double coef = (hig - low) / 2;

    std::vector<double> result(nsteps);
    for (int n = 0; n < nsteps; ++n)
    {
        result.at(n) = term + coef * std::cos(MathConstants::PI * (2.0 * n + 1.0) / (2.0 * nsteps));
    }
    return result;
}
