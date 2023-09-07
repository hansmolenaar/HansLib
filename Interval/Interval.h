#pragma once


#include "MyException.h"
#include "Rational.h"
#include <cmath>

template<typename T>
class Interval
{
public:
   explicit Interval(const T&);
   Interval(const T&, const T&);
   void add(const T&);
   T getLower() const { return m_lower; }
   T getUpper() const { return m_upper; }

   bool contains(const T&) const;
   bool contains(const Interval<T>&) const;

   T getMeasure() const;

   static bool TryIntersect(const Interval<T>&, const Interval<T>&, T&, T&);

   T interpolate(T factor) const;
   T inverseInterpolate(T arg) const;
   auto operator<=>(const Interval<T>&) const = default;

private:
   T m_lower;
   T m_upper;
};

template<typename T>
bool operator==(typename const Interval<T>& lhs, typename const Interval<T>& rhs)
{
   return lhs.getLower() == rhs.getLower() && lhs.getUpper() == rhs.getUpper();
}

template<typename T>
Interval<T>::Interval(const T& val)
{
   m_lower = val;
   m_upper = val;
}


template<typename T>
Interval<T>::Interval(const T& val1, const T& val2) : Interval<T>(val1)
{
   add(val2);
}

template<typename T>
void Interval<T>::add(const T& val)
{
   m_lower = std::min(m_lower, val);
   m_upper = std::max(m_upper, val);
}

template<typename T>
bool Interval<T>::contains(const T& val) const
{
   if (val < m_lower) return false;
   return val <= m_upper;
}

template<typename T>
bool Interval<T>::contains(const Interval<T>& other) const
{
   return (this->getLower() <= other.getLower()) && (this->getUpper() >= other.getUpper());
}

template<typename T>
bool Interval<T>::TryIntersect(const Interval<T>& intv0, const Interval<T>& intv1, T& lo, T& hi)
{
   lo = std::max(intv0.getLower(), intv1.getLower());
   hi = std::min(intv0.getUpper(), intv1.getUpper());
   return lo <= hi;
}

template<typename T>
T Interval<T>::getMeasure() const
{
   return getUpper() - getLower();
}

template<>
inline double Interval<double>::interpolate(double factor) const
{
   return std::lerp(m_lower, m_upper, factor);
}


template<>
inline Rational Interval<Rational>::interpolate(Rational factor) const
{
   return m_lower + factor * (m_upper - m_lower);
}

template<>
inline double Interval<double>::inverseInterpolate(double arg) const
{
   const double shifted = arg - m_lower;
   const double result = shifted / getMeasure();
   if (!std::isfinite(result))
   {
      throw MyException("Interval<double>::inverseInterpolate degenerate interval");
   }
   return result;
}