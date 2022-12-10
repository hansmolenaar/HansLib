#include "BinomialCoefficient.h"
#include "Factorial.h"
#include "BoundsCheck.h"

int BinomialCoefficient::Get(int n, int k)
{
   BoundsCheck<int>::Create(0, n).check(k);
   int result = 1;
   for (int m = 0; m < k; ++m)
   {
      result *= (n - m);
   }
   return result / Factorial::Get(k);
}