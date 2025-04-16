#pragma once

#include <algorithm>
#include <optional>
#include <vector>

namespace Utilities
{
   template <typename C>
   std::optional< typename C::value_type> firstDuplicate(const C& values)
   {
      if (values.size() < 2) return {};

      std::vector<typename C::value_type> sorted(values.begin(), values.end());
      std::sort(sorted.begin(), sorted.end());
      for (size_t n = 1; n < sorted.size(); ++n)
      {
         if (sorted.at(n - 1) == sorted.at(n))
         {
            return { sorted.at(n) };
         }
      }
      return {};
   }

}
