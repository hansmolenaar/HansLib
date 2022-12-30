#pragma once

#include "Assert.h"
#include <vector>

namespace Utilities
{
   template<typename T>
   const T& Single(const std::vector<T>&);

   template<typename T>
   T& Single(std::vector<T>&);

}


template<typename T>
const T& Utilities::Single(const std::vector<T>& container)
{
   Assert(container.size() == 1);
   return container.front();
}

template<typename T>
T& Utilities::Single(std::vector<T>& container)
{
   Assert(container.size() == 1);
   return container.front();
}