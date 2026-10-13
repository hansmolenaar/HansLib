#include "Combinations.h"
#include "Factorial.h"

size_t Utilities::countCombinations(size_t N, size_t r)
{
   if (r > N)
   {
      return 0;
   }
   size_t numerator = 1;
   for (size_t itr = 0; itr < r; ++itr)
   {
      numerator *= (N - itr);
   }
   const size_t denominator = Factorial::Get(r);
   return numerator / denominator;
}
