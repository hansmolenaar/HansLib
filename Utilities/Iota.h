#pragma once
#include <vector>
#include <numeric>

namespace Iota
{
   std::vector<int> GenerateIntVector(size_t siz, int from = 0)
   {
      std::vector<int> result(siz);
      std::iota(result.begin(), result.end(), from);
      return result;
   }
}
