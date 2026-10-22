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

bool Utilities::next_combination(std::vector<int> &a, int n)
{
   int k = (int)a.size();
   for (int i = k - 1; i >= 0; i--)
   {
      if (a[i] < n - k + i + 1)
      {
         a[i]++;
         for (int j = i + 1; j < k; j++)
            a[j] = a[j - 1] + 1;
         return true;
      }
   }
   return false;
}
