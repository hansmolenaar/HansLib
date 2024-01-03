#pragma once

#include <boost/rational.hpp>
#include <boost/functional/hash.hpp>

using Rational = boost::rational<int>;


namespace std
{
   template<>
   struct hash< Rational >
   {
      size_t operator()(const Rational& r) const noexcept
      {
         size_t result = 0;
         boost::hash_combine(result, static_cast<size_t>(r.numerator()));
         boost::hash_combine(result, static_cast<size_t>(r.denominator()));
         return result;
      }
   };

   template<typename T>
   T ToFloat(const Rational& rat)
   {
      return static_cast<T>(rat.numerator()) / rat.denominator();
   }

   inline Rational abs(const Rational& rat)
   {
      if (rat > 0) return rat;
      return -rat;
   }
}