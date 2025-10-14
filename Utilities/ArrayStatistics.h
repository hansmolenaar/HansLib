#pragma once

#include <limits>
#include <span>
#include <sstream>
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

   friend std::ostream& operator<<(std::ostream& os, const ArrayStatistics& stats)
   {
      if (stats.getSize() == 0)
      {
         return os << "EMPTY";
      }

      const std::string sep = "  ";
      os << "NUM=" << stats.getSize()
         << sep << "MIN=" << stats.getMinimum()
         << sep << "MAX=" << stats.getMaximum()
         << sep << "AVG=" << stats.getAverage();

      return os;
   }
private:
   size_t m_size;
   T m_minimum = std::numeric_limits<T>::max();
   T m_maximum = std::numeric_limits<T>::max();
   double m_average = std::numeric_limits<double>::quiet_NaN();
};