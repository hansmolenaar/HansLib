#pragma once

#include "MyAssert.h"
#include <array>
#include <cmath>

// 3  4  5  6a          9 10 11 12
// 2  3  4  5           4  5  6 13
// 1  2  3  4   =>      1  2  7 14
// 0  1  2  3           0  3  8 15 
// i ->                

template <typename T>
class Indexer2 \
{
public:
   static size_t ToFlat(T i, T j);
   static std::array<T, 2> FromFlat(size_t flat);
};

template <typename T>
size_t Indexer2<T>::ToFlat(T i, T j)
{
   Utilities::MyAssert(std::min(i, j) >= 0);
   if (j >= i) return j * j + i;
   return i * i + 2 * i - j;
}


template <typename T>
std::array<T, 2> Indexer2<T>::FromFlat(size_t flat)
{
   // add something to avoid round-off problems
   const double md = std::floor(std::sqrt(flat + 0.1));
   const T m = static_cast<T>(md);
   const T diag = m * m + m;
   if (flat <= diag) return std::array<T, 2>{static_cast<T>(flat - m * m), m};
   return std::array<T, 2>{m, static_cast<T>(m* m + 2 * m - flat)};
}