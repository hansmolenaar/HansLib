#pragma once
#include "BoundsCheck.h"

namespace Factorial
{
template <typename T> T Get(T);
}

template <typename T> T Factorial::Get(T number)
{
    BoundsCheck<T>::CreateIsNonNegative().check(number);
    T result = 1;
    for (T n = 1; n <= number; ++n)
    {
        result *= n;
    }
    return result;
}