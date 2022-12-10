#include "Factorial.h"
#include "BoundsCheck.h"
#include <vector>

int Factorial::Get(int number)
{
   BoundsCheck<int>::CreateIsNonNegative().check(number);
   int result = 1;
   for (int n = 1; n <= number; ++n)
   {
      result *= n;
   }
   return result;
}