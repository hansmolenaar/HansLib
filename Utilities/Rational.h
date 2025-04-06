#pragma once

#include <boost/functional/hash.hpp>
#include <boost/rational.hpp>
#include  <span>

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

   inline Rational abs(const Rational& rat)
   {
      if (rat > 0) return rat;
      return -rat;
   }
}

inline double operator*(double d, Rational r)
{
   return d * r.numerator() / r.denominator();
}


inline double operator*(Rational r, double d)
{
   return d * r.numerator() / r.denominator();
}

inline std::ostream& operator<< (std::ostream& os, const Rational& rat)
{
   if (rat == Rational(0, 1))
   {
      os << 0;
   }
   else if (rat.denominator() == 1)
   {
      os << rat.numerator();
   }
   else
   {
      os << rat.numerator() << "/" << rat.denominator();
   }
   return os;
}


inline std::ostream& operator<< (std::ostream& stream, std::span<const Rational> rat)
{
   stream << "(";
   bool first = true;
   for (auto r : rat)
   {
      if (!first) stream << ", ";
      first = false;

      stream << r;
   }
   stream << ")";
   return stream;
}