#pragma once

#include "IntervalTreeStatistics.h"

namespace IntervalTree
{
   std::string Statistics::toString() const
   {
      std::ostringstream result;
      result << Size;
      result << ", {";
      bool first = true;
      for (auto n : NumberOfLeavesPerLevel)
      {
         if (!first)
         {
            result << ", ";
         }
         first = false;
         result << n;
      }
      result << "}";
      return result.str();
   }
}
