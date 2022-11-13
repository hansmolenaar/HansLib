#include "EquidistantPoints.h"
#include"Utilities/Assert.h"

std::vector<double> EquidistantPoints::Generate(double low, double hig, int nsteps)
{
   Utilities::Assert(hig > low);
   Utilities::Assert(nsteps > 0);
   const double del = (hig - low) / nsteps;
   std::vector<double> result(nsteps+1);
   for (int n = 0; n <= nsteps; ++n)
   {
      result.at(n) = low + n * del;
   }
   return result;
}