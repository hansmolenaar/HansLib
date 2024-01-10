#include "ArrayStatistics.h"
#include "Rational.h"

#include <algorithm>
#include <numeric>
#include <iostream>

template ArrayStatistics<int>;
template ArrayStatistics<double>;
template ArrayStatistics<Rational>;

template<typename T>
ArrayStatistics<T>::ArrayStatistics(std::span<const T> values) : 
   m_size(values.size())
{
   if (m_size == 0) return;
   const auto [minElm, maxElm] = std::minmax_element(values.begin(), values.end());
   m_minimum = *minElm;
   m_maximum = *maxElm;
   T sum = 0;
   sum = std::accumulate(values.begin(), values.end(), sum);
   m_average = (1.0 * sum) / m_size;
}

template<typename T>
size_t ArrayStatistics<T>::getSize() const
{
   return m_size;
}

template<typename T>
T ArrayStatistics<T>::getMinimum() const
{
   return m_minimum;
}

template<typename T>
T ArrayStatistics<T>::getMaximum() const
{
   return m_maximum;
}

template<typename T>
double ArrayStatistics<T>::getAverage() const
{
   return m_average;
}

template<typename T>
std::string ArrayStatistics<T>::toString() const
{
   const std::string sep = "  ";
   std::ostringstream oss;
   oss        << "NUM=" << getSize()
       << sep << "MIN=" << getMinimum()
       << sep << "MAX=" << getMaximum()
       << sep << "AVG=" << getAverage();
   return oss.str();
}