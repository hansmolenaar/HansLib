#pragma once

#include "MyException.h"
#include <algorithm>
#include <optional>
#include <sstream>
#include <vector>

namespace Utilities
{
   template <typename C>
   std::optional< typename C::value_type> firstDuplicate(const C& values)
   {
      if (values.size() < 2) return {};
      if (values.size() == 2)
      {
         if (values[0] == values[1]) return { values[0] };
         return {};
      }
      if (values.size() == 3)
      {
         if (values[0] == values[1] || values[0] == values[2]) return { values[0] };
         if (values[1] == values[2]) return { values[1] };
         return {};
      }
      if (values.size() == 4)
      {
         if (values[0] == values[1] || values[0] == values[2] || values[0] == values[3]) return { values[0] };
         if (values[1] == values[2] || values[1] == values[3]) return { values[1] };
         if (values[2] == values[3]) return { values[2] };
         return {};
      }

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

   template <typename C>
   void throwOnDuplicate(const C& values)
   {
      const auto found = firstDuplicate(values);
      if (found)
      {
         std::ostringstream msg;
         msg << "Container has duplicate: " << *found;
         throw MyException(msg.str());
      }
   }
}
