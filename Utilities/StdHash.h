#pragma once

#include "Rational.h"
#include <array>
#include <boost/functional/hash.hpp>

namespace std
{
   template <typename T, int N>
   struct hash< std::array<T, N> > {
      size_t operator()(const std::array<T, N>& key) const noexcept
      {
         size_t result = 0;
         for (int n = 0; n < N; ++n)
         {
            boost::hash_combine(result, std::hash<T>{}(key[n]));
         }
         return result;
      }
   };

}