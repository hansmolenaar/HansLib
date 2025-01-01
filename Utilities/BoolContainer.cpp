#include "BoolContainer.h"
#include "BoundsCheck.h"

BoolContainer BoolContainerUtils::FromNumber(int number)
{
   BoundsCheck<int>::CreateIsNonNegative().check(number);

   if (number == 0) return { false };
   BoolContainer result;

   while (number > 0)
   {
      const int bit = number % 2;
      result.push_back(bit);
      number /= 2;
   }
   std::reverse(result.begin(), result.end());
   return result;
}

int BoolContainerUtils::ToNumber(const BoolContainer& bools)
{
   int result = 0;
   int pow2 = 1;
   for (auto itr = bools.rbegin(); itr != bools.rend(); ++itr)
   {
      if (*itr) result += pow2;
      pow2 *= 2;
   }
   return result;

}