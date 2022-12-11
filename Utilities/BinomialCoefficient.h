#pragma once
#include "Factorial.h"

namespace BinomialCoefficient
{
   template <typename T>
   T Get(T n, T k);
}

template <typename T>
T BinomialCoefficient::Get(T n, T k)
{
   BoundsCheck<T>::Create(0, n).check(k);
   T result = 1;
   for (T m = 0; m < k; ++m)
   {
      result *= (n - m);
   }
   return result / Factorial::Get(k);
}