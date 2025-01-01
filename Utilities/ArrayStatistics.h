#pragma once

#include <limits>
#include <span>
#include <string>

template<typename T>
class ArrayStatistics
{
public:
   explicit ArrayStatistics(std::span<const T> values);
   size_t getSize() const;
   T getMinimum() const;
   T getMaximum() const;
   double getAverage() const;
   std::string toString() const;
private:
   size_t m_size;
   T m_minimum = std::numeric_limits<T>::max();
   T m_maximum = std::numeric_limits<T>::max();
   double m_average = std::numeric_limits<double>::quiet_NaN();
};